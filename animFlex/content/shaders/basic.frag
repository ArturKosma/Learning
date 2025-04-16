#version 300 es
precision mediump float;

uniform sampler2D Tex;

in vec2 UV;

layout (location = 0) out vec4 FragColor;

void main()
{	
    FragColor = texture(Tex, UV);
}