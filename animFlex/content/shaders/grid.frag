#version 300 es
precision highp float;

layout (std140) uniform Camera
{
	mat4 cameraTransform;
};

layout (std140) uniform RenderProperties
{
	mat4 renderProperties;
};

in vec2 UV;
in float CellNum;
in vec3 WorldPosition;

layout (location = 0) out vec4 FragColor;

int MaxSamples = 10;

float LinearizeDepth(float d, float zNear, float zFar)
{
	return (2.0f * zNear * zFar) / (zFar + zNear - (d * 2.0f - 1.0f) * (zFar - zNear));
}

float GridLineGenerationAliased(vec2 uv)
{
	float minLineWidthX = 0.015f;  
	float minLineWidthY = 0.015f;

	// Get the distance from the center of UV tile.
	float distX = abs(uv.x - 0.5f);
	float distY = abs(uv.y - 0.5f);

	// Smoothly transition between 0.0f and 1.0f for X and Y lines,
	float lineX = smoothstep(minLineWidthX, 0.0f, distX);
	float lineY = smoothstep(minLineWidthY, 0.0f, distY);

	// Sum the lines.
	float line = clamp(lineX + lineY, 0.0f, 1.0f);

	return line;
}

float SampleGridWithFilter(vec2 uv, float cellSize, float detail)
{
    vec2 cellUV = uv * cellSize;
    vec2 dCellUVx = dFdx(uv * cellSize);
	vec2 dCellUVy = dFdy(uv * cellSize);

	int sx = 1 + clamp(int(detail * length(dCellUVx - cellUV)), 0, MaxSamples - 1);
    int sy = 1 + clamp(int(detail * length(dCellUVy - cellUV)), 0, MaxSamples - 1);

    float gridSum = 0.0;

    for(int j = 0; j < sy; j++)
	{	
		for(int i = 0; i < sx; i++)
		{
			vec2 st = vec2(float(i), float(j)) / vec2(float(sx), float(sy));
			vec2 sampleUV = cellUV + st.x * dCellUVx + st.y * dCellUVy;

			gridSum += GridLineGenerationAliased(fract(sampleUV));
		}
	}

    return gridSum / float(sx * sy);
}

float Remap(float value, float inMin, float inMax, float outMin, float outMax)
{
	float mappedValue = (value - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;

	return clamp(mappedValue, min(outMin, outMax), max(outMin, outMax));
}

float Spotlight(vec3 fragWorldPos, vec3 spotlightWorldPosition, vec3 spotlightWorldDirection, float distFromCamera)
{
	vec3 toPoint = fragWorldPos - spotlightWorldPosition;
	vec3 crossProduct = cross(toPoint, normalize(spotlightWorldDirection));
	float calc = length(crossProduct) / length(spotlightWorldDirection);

	return calc;
}

// Hash a UV location using pseudo-random hasher.
float hash(vec2 p) 
{
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

void main()
{	
	vec3 cameraPos = cameraTransform[3].xyz;
	vec3 cameraFwd = normalize(vec3(cameraTransform[2].xyz));
	float fragDist = length(cameraPos - WorldPosition);
	
	// Cache near/far planes.
	float near = renderProperties[0][2];
	float far = renderProperties[0][3];

	// Generate grid lines.
	float line = SampleGridWithFilter(UV + 0.5f, CellNum, 2.0f);

	// Clamp the brightness line.
	float lineMask = clamp(line, 0.0f, 0.7f);

	// Find line distance.
	float lineDist = Remap(fragDist, 10.0f, 500.0f, 0.0f, 1.0f);
	lineDist = exp(-20.0f * lineDist);

	// Lower the line brightness with distance.
	lineMask *= lineDist;

	// Lower the brightness with height.
	float lineHeightMaxRadius = Remap(cameraPos.y, 0.0f, 10.0f, 10.0f, 300.0f);
	float gridBrightnessHeightModifier = smoothstep(0.0f, lineHeightMaxRadius, fragDist);
	gridBrightnessHeightModifier = exp(-pow(gridBrightnessHeightModifier, 2.0f) / 0.3f) * (1.0f - gridBrightnessHeightModifier);

	vec2 flatDist = vec2(abs(WorldPosition.x - cameraPos.x), abs(WorldPosition.z - cameraPos.z));
	float flatDistX = Remap(flatDist.x, 0.0f, 300.0, 0.0f, 1.0f);
	float flatDistY = Remap(flatDist.y, 0.0f, 300.0f, 0.0f, 1.0f);
	flatDist = vec2(flatDistX, flatDistY) - (flatDistX * flatDistY) * 300.0f / 1.3f;
	float dist45 = length(flatDist);
	dist45 = clamp((1.0f / exp(dist45)) * (2.0f - dist45), 0.2f, 1.0f);

	// Final line color.
	float lineColor = clamp(-0.45f * lineMask * gridBrightnessHeightModifier * dist45, -1.0f, 0.0f);

	// Faked spotlight in the direction of camera.
	vec3 s_light_cameraPos = vec3(cameraPos.x, 20.0f, cameraPos.z);
	vec3 s_light_cameraDir = normalize(vec3(cameraFwd.x, 0.0f, cameraFwd.z));
	float s_light_radius = 20.0f;
	float s_light_dist = Spotlight(WorldPosition, s_light_cameraPos, s_light_cameraDir, fragDist);
	float s_light_attenuation = s_light_radius / s_light_dist;
	float s_light_distModifier = Remap(fragDist, 5.0f, 200.0f, 0.0f, 1.0f);
	s_light_distModifier = exp(-5.0f * s_light_distModifier) + 0.1f;
	float s_light = (s_light_attenuation * s_light_distModifier) + 0.2f;

	// Add all lights together.
	float lights = s_light;

	// Screen space for vigniette on the grid.
	vec2 screenSpaceUV = gl_FragCoord.xy;

	// Fetch uniform resolution.
	vec2 frameRes = vec2(renderProperties[0][0], renderProperties[0][1]);

	// Get proper screen space uv.
	screenSpaceUV /= frameRes;

	// Get radial distance from center of the screen.
	float screenSpaceDist = length(abs((screenSpaceUV - 0.5f) * 2.0f));

	// Radial vigniette.
	float vigniette = exp(-1.0f * pow(screenSpaceDist, 2.0f));

	// Total brightness modifier.
	float brightnessModifier = lights * vigniette;

	// Define base quad color.
	vec4 baseColor = vec4(0.259f, 0.4f, 0.439f, 1.0f) * 1.4f;

	// Compute final color.
	vec4 finalColor = (baseColor + lineColor) * brightnessModifier;

	// Create noise for dithering (needed to get rid of banding).
	float noise = hash(UV * frameRes);	

	// Dither final color to get rid of banding.
	finalColor += (noise - 0.5f) * 0.02f;

	//finalColor = vec4(UV, 0.0f, 1.0f);

	FragColor = finalColor;
}