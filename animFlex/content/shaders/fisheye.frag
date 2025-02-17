#version 300 es
precision mediump float;

uniform sampler2D u_ColorTex;
uniform sampler2D u_DepthTex;

layout (std140) uniform InverseViewProjection
{
	mat4 inverseView;
	mat4 inverseProjection;
};

layout (std140) uniform ViewProjection
{
	mat4 view;
	mat4 projection;
};

layout (std140) uniform Camera
{
	mat4 cameraTransform;
};

layout (std140) uniform RenderProperties
{
	mat4 renderProperties;
};

layout (location = 0) out vec4 FragColor;

float LinearizeDepth(float d, float zNear, float zFar)
{
	return (2.0f * zNear * zFar) / (zFar + zNear - (d * 2.0f - 1.0f) * (zFar - zNear));
}

// Given a vec2 in <-1.0f, 1.0f>, generate a texture coord in <0.0f, 1.0f>.
vec2 BarrelDistortion(vec2 p)
{
	const float barrelPowerMaxOffset = 0.2f;

	float theta  = atan(p.y, p.x);
    float radius = length(p);
    radius = pow(radius, 1.0f + barrelPowerMaxOffset);
    p.x = radius * cos(theta);
    p.y = radius * sin(theta);
    return 0.5 * (p + 1.0);
}

float Remap(float value, float inMin, float inMax, float outMin, float outMax)
{
	float mappedValue = (value - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;

	return clamp(mappedValue, min(outMin, outMax), max(outMin, outMax));
}

void main()
{	
	vec2 frameRes = vec2(renderProperties[0][0], renderProperties[0][1]);

	float near = renderProperties[0][2];
	float far = renderProperties[0][3];

	// Get screen space UV.
    vec2 uv = gl_FragCoord.xy / frameRes;

	vec4 depth = texture(u_DepthTex, uv);
	float depthLin = LinearizeDepth(depth.x, near, far) / far;

	vec4 screen = texture(u_ColorTex, uv);
	vec4 finalColor = vec4(vec3(screen.xyz), 1.0f);

	//finalColor = vec4(vec3(depthLin), 1.0f);

	FragColor = finalColor;
}