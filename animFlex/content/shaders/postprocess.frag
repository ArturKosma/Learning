#version 300 es
precision highp float;

uniform sampler2D u_ColorTex;
uniform sampler2D u_DepthTex;
uniform float u_zNear;
uniform float u_zFar;

in vec4 TexColor;
in vec2 TexCoord;

layout (location = 0) out vec4 FragColor;

float RandomNoise(vec2 uv)
{
    return fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
}

float LinearizeDepth(float d, float zNear, float zFar)
{
	return (2.0f * zNear) / (zFar + zNear - d * (zFar - zNear));
}

void main()
{	
	float depth = texture(u_DepthTex, TexCoord).x;
	float depthLin = LinearizeDepth(depth, u_zNear, u_zFar);

	vec4 screen = texture(u_ColorTex, TexCoord);
	vec4 finalColor = vec4(vec3(screen.xyz), 1.0f);

	float noise = RandomNoise(TexCoord) * 0.6;
	float ditheredDepth = depthLin + noise;

	//FragColor = finalColor;
	FragColor = vec4(vec3(depthLin), 1.0f);
}