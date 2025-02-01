#version 300 es
precision mediump float;

in vec4 texColor;
in vec2 texCoord;

layout (location = 0) out vec4 FragColor;

uniform sampler2D Tex;

void main()
{	
	FragColor = vec4(vec3(1.0f, 0.0f, 0.0f), 1.0f);
	//FragColor = texture(Tex, texCoord);
}