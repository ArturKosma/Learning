#version 300 es
precision highp float;

flat in uvec4 UniqueID;

layout (location = 0) out uvec4 FragColor;

void main()
{	
    FragColor = UniqueID;
}