#version 300 es
precision mediump float;
precision mediump usampler2D;

uniform usampler2D u_ColorTex;

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

	uvec4 screen = texture(u_ColorTex, uv);

	FragColor = vec4(screen) / 255.0f;
}