#version 300 es
precision highp float;
precision highp int;

layout (std140) uniform UITransform
{
    mat4 uiTransform;
};

layout (std140) uniform OrthoProjection
{
    mat4 orthoProjection;
};

layout (std140) uniform ViewRotation
{
	mat4 viewRotation;
};

uniform float uBrightnessFaces[32];

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in uvec4 aID;
layout (location = 4) in vec2 aUVCenter;

out vec3 Pos;
out vec2 UV;
flat out uvec4 ID;
flat out float brightness;
out float normBrightness;
out float distBrightness;

void main()
{
	Pos = aPos;
	UV = aUV;
	ID = aID;

	brightness = uBrightnessFaces[aID.b];

	// Find position after rotation.
	vec4 rotatedPos = viewRotation * vec4(aPos, 1.0f);

	// Find normal after rotation.
	vec4 rotatedNorm = vec4(normalize((viewRotation * vec4(aNormal, 0.0f)).xyz), 0.0f);

	// Find brightness based on normal.
	normBrightness = clamp(dot(vec3(0.0f, 0.0f, 1.0f), rotatedNorm.xyz), 0.5f, 0.8f);
	//normBrightness = 1.0f;

	// Find brightness based on distance (deeper = darker).
	distBrightness = clamp(rotatedPos.z + 0.75f, 0.5f, 1.1f);
	//distBrightness = 1.0f;

	// Fake perspective in ortho view.
	float fakePerspectiveScale = 1.0f + rotatedPos.z * 0.2f;
    rotatedPos.xy *= fakePerspectiveScale;

	// Fix UV after fake perspective.
	UV = (aUV - aUVCenter) * fakePerspectiveScale * 0.9f + aUVCenter;

	gl_Position = uiTransform * orthoProjection * rotatedPos;
}