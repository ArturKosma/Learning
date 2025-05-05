#version 300 es
precision mediump float;

uniform sampler2D Tex;

flat in int glyphID;
const vec3 glyphColors[3] = vec3[]
    (
        vec3(1.0f, 0.0f, 0.0f),
        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 0.0f, 1.0f)
    );

in vec2 UV;

layout (location = 0) out vec4 FragColor;

void main()
{	
    vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(Tex, UV).r);
    FragColor = vec4(glyphColors[glyphID], 1.0f) * sampled;
}