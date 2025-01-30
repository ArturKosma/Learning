#version 300 es
precision mediump float;

layout (location = 0) out vec4 FragColor;

uniform sampler2D Tex;

void main()
{	
	FragColor = vec4(1.0f);
}