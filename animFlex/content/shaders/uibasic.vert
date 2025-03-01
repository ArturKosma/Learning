#version 300 es
precision mediump float;

layout (std140) uniform UITransform
{
	mat4 uiTransform;
};

layout (location = 0) in vec3 aPos;

void main()
{
	gl_Position = vec4(aPos, 1.0);
}