#version 300 es
precision mediump float;

uniform sampler2D u_ColorTex;

layout (std140) uniform RenderProperties
{
	mat4 renderProperties;
};

layout (location = 0) out vec4 FragColor;

void main()
{	
	vec2 res = vec2(renderProperties[0][0], renderProperties[0][1]);

	// Get screen space UV.
    vec2 uv = gl_FragCoord.xy / res;

	vec4 screen = texture(u_ColorTex, uv);

	FragColor = screen;
}