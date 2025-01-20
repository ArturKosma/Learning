#version 300 es
precision mediump float;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 texColor;
out vec2 texCoord;

void main()
{
	vec3 offset = vec3(0.1, 0.1, 0.0);
	gl_Position = vec4(aPos + offset, 1.0);
	texColor = vec4(aColor, 1.0);
	texCoord = aTexCoord;
}