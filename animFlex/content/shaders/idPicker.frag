#version 300 es
precision highp float;

flat in uvec4 ID;

layout (location = 0) out uvec4 FragColor;

void main()
{	
    FragColor = ID;
}