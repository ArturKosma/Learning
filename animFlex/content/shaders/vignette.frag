#version 300 es
precision mediump float;

uniform sampler2D u_ColorTex;
uniform sampler2D u_DepthTex;

layout (std140) uniform RenderProperties
{
	mat4 renderProperties;
};

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
	vec2 res = vec2(renderProperties[0][0], renderProperties[0][1]);
	float near = renderProperties[0][2];
	float far = renderProperties[0][3];

	// Get screen space UV.
    vec2 uv = gl_FragCoord.xy / res;

	float depth = texture(u_DepthTex, uv).x;
	float depthLin = LinearizeDepth(depth, near, far);

	vec4 screen = texture(u_ColorTex, uv);
	vec4 finalColor = vec4(vec3(screen.xyz), 1.0f);

	float noise = RandomNoise(uv) * 0.6;
	float ditheredDepth = depthLin + noise;

	FragColor = finalColor;
}