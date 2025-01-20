#version 300 es
precision mediump float;

in vec4 texColor;
in vec2 texCoord;

uniform sampler2D Tex;

layout (location = 0) out vec4 FragColor;

void main()
{	
	vec4 texFetch = texture(Tex, texCoord);

	float r = texFetch.r * (1.0 - texColor.r);
	float g = texFetch.g * (1.0 - texColor.g);
	float b = texFetch.b * (1.0 - texColor.b);

	FragColor = vec4(r, g, b, 1.0);
}