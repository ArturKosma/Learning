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
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

out vec3 Normal;
out vec2 UV;

void main()
{
	Normal = aNormal;
	UV = aUV;
	gl_Position = projection * view * modelTransform * vec4(aPos, 1.0f);
}