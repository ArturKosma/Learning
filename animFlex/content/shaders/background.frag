#version 300 es
precision highp float;

layout (std140) uniform InverseViewProjection
{
	mat4 inverseView;
	mat4 inverseProjection;
};

uniform vec2 u_Resolution;
uniform vec3 u_CameraPos;

layout (location = 0) out vec4 FragColor;

void main()
{	
	// Get screen space UV.
    vec2 uv = gl_FragCoord.xy / u_Resolution;

	// Get UV <0.0f, 1.0f> into Normalized Device Coordinates <-1.0f, 1.0f>. We need sign for proper vector direction.
	vec2 ndc = (uv - 0.5f) * 2.0f;

	// Aspect ratio correction.
	ndc.x *= (u_Resolution.y / u_Resolution.x);

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
	float offset = (u_CameraPos.y * 0.0093f) - 0.01f;

	// Get the Y, which is height of the ray.
	float height = clamp(worldDir.y + offset, 0.0f, 1.0f);

	// Background color.
	vec3 backgroundColor = vec3(0.104f, 0.162f, 0.2f) * 0.6f;

	// Horizon color.
	vec3 horizonColor = (vec3(0.104f, 0.162f, 0.188f) + 0.2f) * 2.0f;

	// Create horizon brightness.
	float horizonBrightness = (1.0f / (1.0f - pow(abs(height - 0.93f), 8.0))) - 1.0f;

	// Create horizon.
	vec3 horizon = vec3(horizonBrightness * horizonColor);

	// Colorize!
	FragColor = vec4(backgroundColor + horizon, 1.0f);
}