#version 300 es
precision mediump float;

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

out vec2 UV;

void main()
{
	UV = aUV;
	gl_Position = projection * view * modelTransform * vec4(aPos, 1.0);
}