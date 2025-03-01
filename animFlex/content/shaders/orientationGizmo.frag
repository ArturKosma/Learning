#version 300 es
precision mediump float;

in vec3 Pos;

layout (location = 0) out vec4 FragColor;

void main()
{	
    float red = Pos.x;
    float green = Pos.y;
    float blue = Pos.z;

    FragColor = vec4(vec3(red, green, blue), 1.0f);
}