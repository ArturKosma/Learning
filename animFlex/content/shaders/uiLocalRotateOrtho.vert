#version 300 es
precision mediump float;

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
layout (location = 2) in vec2 aUV;
layout (location = 3) in uvec4 aID;

out vec3 Pos;
out vec2 UV;
flat out uvec4 ID;
flat out float brightness;

void main()
{
	Pos = aPos;
	UV = aUV;
	ID = aID;

	brightness = uBrightnessFaces[aID.b];

	gl_Position = uiTransform * orthoProjection * viewRotation * vec4(aPos, 1.0f);
}