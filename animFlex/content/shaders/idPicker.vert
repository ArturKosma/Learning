#version 300 es
precision highp float;

layout (std140) uniform ViewProjection
{
	mat4 view;
	mat4 projection;
};

layout (std140) uniform Model
{
	mat4 modelTransform;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in uvec4 aID;

flat out uvec4 ID;

void main()
{
	ID = aID;
	gl_Position = projection * view * modelTransform * vec4(aPos, 1.0f);
}