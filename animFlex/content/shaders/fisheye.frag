#version 300 es
precision highp float;

uniform sampler2D u_ColorTex;
uniform sampler2D u_DepthTex;
uniform sampler2D u_StencilTex;

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

float GetHorizonMask(vec2 uv, vec2 frameRes)
{
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
	float offset = (cameraPos.y * 0.00005f) - 0.0075f;

	// Get the Y, which is height of the ray.
	float correction = 0.5f;
	float height = clamp(worldDir.y + offset + correction, 0.0f, 1.0f); // Height is shifted down.
	float heightCeil = ceil(height);
	float horizonMask = clamp(height + ((1.0f - correction * 2.0f) * heightCeil), 0.0f, 1.0f); // Everything above 0.1f will end up 1.0f;
	horizonMask = abs(horizonMask - (1.0f * heightCeil)); // Everything that remained below 1.0f will become negative, we abs it.
	horizonMask = ceil(horizonMask); // We ceil it to get a uniform 1.0f on a thin patch.

	// Make the line patch gradient out from the center.
	height = smoothstep(0.0f, correction * 2.0f, height);
	//height = 1.0f - abs(height - 0.5f) * 2.0f;

	return height * horizonMask;
}

float Remap(float value, float inMin, float inMax, float outMin, float outMax)
{
	float mappedValue = (value - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;

	return clamp(mappedValue, min(outMin, outMax), max(outMin, outMax));
}

float Sigmoid(float x, float k)
{
	return 1.0f / (1.0f + exp(-k * (x - 0.5f)));
}

void main()
{	
	// Fetch uniform resolution.
	vec2 frameRes = vec2(renderProperties[0][0], renderProperties[0][1]);

	// Get screen space UV.
    vec2 uv = gl_FragCoord.xy / frameRes;

	// Stencil of objects used for occlusion.
	float stencil = 1.0f - texture(u_StencilTex, uv).r;

	// Find horizon mask.
	float horizonMask = GetHorizonMask(uv, frameRes);
	float mask = horizonMask;

	// Horizon mask can be occluded.
	horizonMask *= stencil;

	// The further away from the horizontal center of the screen something is - the stronger the effect.
	float horizontalModifier = 0.0f;
	horizontalModifier = abs((uv.x - 0.5f) * 2.5f); // Distance.
	horizontalModifier = pow(horizontalModifier, 2.0f); // Distance non linear.
	horizonMask = horizonMask * horizontalModifier / 2.0f;
	horizonMask *= 1.0f;

	uv += vec2(0.0f, horizonMask * 0.1f);

	vec4 screen = texture(u_ColorTex, uv);
	//vec4 finalColor = vec4(horizonMask > 0.0f ? vec3(horizonMask) : vec3(screen.xyz), 1.0f);
	vec4 finalColor = screen;
	//vec4 finalColor = vec4(vec3(stencil), 1.0f);


	FragColor = finalColor;
}