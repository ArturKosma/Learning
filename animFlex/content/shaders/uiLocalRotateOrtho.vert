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

out vec3 Pos;

void main()
{
	Pos = aPos;
	gl_Position = uiTransform * orthoProjection * viewRotation * vec4(aPos, 1.0);
}