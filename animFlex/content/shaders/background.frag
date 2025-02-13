#version 300 es
precision highp float;

layout (std140) uniform InverseViewProjection
{
	mat4 inverseView;
	mat4 inverseProjection;
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

float hash(vec2 p) 
{
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

void main()
{	
	// Fetch uniform resolution.
	vec2 frameRes = vec2(renderProperties[0][0], renderProperties[0][1]);

	// Get screen space UV.
    vec2 uv = gl_FragCoord.xy / frameRes;

	// Get UV <0.0f, 1.0f> into Normalized Device Coordinates <-1.0f, 1.0f>. We need sign for proper vector direction.
	vec2 ndc = (uv - 0.5f) * 2.0f;

	// Aspect ratio correction.
	ndc.x *= (frameRes.y / frameRes.x);

	// We need NDC in homogeneous. We apply Z = 1.0f to indicate it's into/outo the screen.
	vec4 clipPos = vec4(ndc.x, ndc.y, 1.0f, 1.0f);

	// Convert to view space by multiplying with inverse projection.
	vec4 viewPos = inverseProjection * clipPos;

	// Leave homogeneous and get direction.
	vec3 viewDir = normalize(viewPos.xyz / viewPos.w);

	// Get world direction of the ray.
	vec3 worldDir = normalize((inverseView * vec4(viewDir, 0.0f)).xyz);

	// Calculate an offset based on camera height.
	// This is very arbitrary but I can't come up with a better solution yet.
	vec3 cameraPos = cameraTransform[3].xyz;
	float offset = (cameraPos.y * 0.005f) - 0.01f;

	// Get the Y, which is height of the ray.
	float height = clamp(worldDir.y + offset, 0.0f, 1.0f);

	// Background color.
	vec3 backgroundColor = vec3(0.047f, 0.078f, 0.106f) * 1.0f;

	// Horizon color.
	vec3 horizonColor = (vec3(0.259f, 0.4f, 0.439f) + 0.2f) * 1.2f;

	// Create horizon brightness.
	float horizonBrightness = (1.0f / (1.0f - pow(abs(height - 0.93f), 8.0))) - 1.0f;

	// Create horizon.
	vec3 horizon = vec3(horizonBrightness * horizonColor);

	// Create noise for dithering (needed to get rid of banding).
	float noise = hash(worldDir.xy * frameRes);

	// Add the dither noise to horizon brightness mask.
	horizon += (noise - 0.5f) * 0.02f;

	// Colorize!
	FragColor = vec4(backgroundColor + horizon, 1.0f);
}