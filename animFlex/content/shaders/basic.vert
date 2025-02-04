#version 300 es
precision mediump float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (std140) uniform ViewProjection
{
	mat4 view;
	mat4 projection;
};

layout (std140) uniform Model
{
	mat4 modelTransform;
};

out vec4 texColor;
out vec2 texCoord;

void main()
{
	gl_Position = projection * view * modelTransform * vec4(aPos, 1.0);
	texColor = vec4(aColor, 1.0);
	texCoord = aTexCoord;
}