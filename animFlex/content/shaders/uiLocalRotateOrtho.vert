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

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in uvec4 aID;

out vec3 Pos;
out vec2 UV;
flat out uvec4 ID;

void main()
{
	Pos = aPos;
	UV = aUV;
	ID = aID;
	gl_Position = uiTransform * orthoProjection * viewRotation * vec4(aPos, 1.0);
}