#version 300 es
precision mediump float;

layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices
{
	mat4 view;
	mat4 projection;
	mat4 modelTransform;
};

void main()
{
	gl_Position = projection * view * vec4(aPos, 1.0);
}