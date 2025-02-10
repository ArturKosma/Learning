#version 300 es
precision mediump float;

uniform sampler2D u_ColorTex;
uniform sampler2D u_DepthTex;
uniform float u_zNear;
uniform float u_zFar;
uniform vec2 u_Resolution;

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
	// Get screen space UV.
    vec2 uv = gl_FragCoord.xy / u_Resolution;

	float depth = texture(u_DepthTex, uv).x;
	float depthLin = LinearizeDepth(depth, u_zNear, u_zFar);

	vec4 screen = texture(u_ColorTex, uv);
	vec4 finalColor = vec4(vec3(screen.xyz), 1.0f);

	float noise = RandomNoise(uv) * 0.6;
	float ditheredDepth = depthLin + noise;

	FragColor = finalColor;
	//FragColor = vec4(vec3(finalColor.x), 1.0f);
	//FragColor = vec4(vec3(depthLin), 1.0f);
}