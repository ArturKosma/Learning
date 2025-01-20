#version 300 es
precision mediump float;

in vec4 texColor;
in vec2 texCoord;

layout (location = 0) out vec4 FragColor;

uniform sampler2D Tex;

void main()
{	
	FragColor = texture(Tex, texCoord) * texColor;
}