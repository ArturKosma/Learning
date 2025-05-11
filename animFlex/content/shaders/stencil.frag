#version 300 es
precision mediump float;

layout (location = 0) out vec4 FragColor;

void main()
{	
	FragColor = vec4(vec3(1.0f), 1.0f); // Where stencil test passes, write white.
}