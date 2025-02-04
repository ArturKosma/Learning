#version 300 es
precision highp float;

uniform sampler2D u_ColorTex;
uniform sampler2D u_DepthTex;

in vec4 TexColor;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;

void main()
{	
	float line = 0.0f;
	if(TexCoord.x < 0.51 && TexCoord.x > 0.49) line = 1.0f;
	line = 0.0f;
	float depth = texture(u_DepthTex, TexCoord).r;

	vec4 screen = texture(u_ColorTex, TexCoord);
	vec4 finalColor = vec4(vec3(screen.xyz + line), 1.0f);

	FragColor = finalColor;
	//FragColor = vec4(vec3(depth), 1.0f);
}