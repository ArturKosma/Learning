#version 300 es
precision highp float;

uniform sampler2D u_ColorTex;
uniform sampler2D u_DepthTex;

layout (std140) uniform RenderProperties
{
	mat4 renderProperties;
};

layout (location = 0) out vec4 FragColor;

// Gaussian Blur lookup table.
// 7x7 according to chatGPT.
const float weights[4] = float[](0.27407, 0.22408, 0.09145, 0.01622);

void main()
{	
	// Get resolution.
	vec2 res = vec2(renderProperties[0][0], renderProperties[0][1]);

	// Get screen space UV.
    vec2 uv = gl_FragCoord.xy / res;

	// Get texel width and height.
	float texelWidth = 1.0f / res.x;
	float texelHeight = 1.0f / res.y;

	// Get depth modifier.
	// It controls the lerp between blur and clean image.
	float depth = texture(u_DepthTex, uv).x;
	depth = smoothstep(0.9f, 1.0f, depth);

	// Get center color.
	vec3 sharp = texture(u_ColorTex, uv).rgb;
	vec3 blur = texture(u_ColorTex, uv).rgb * weights[0];

	// Add kernel colors.
	for(int i = 1; i < 4; ++i)
	{
		blur += texture(u_ColorTex, vec2(uv.x + (texelWidth * float(i)), uv.y)).rgb * weights[i];
		blur += texture(u_ColorTex, vec2(uv.x - (texelWidth * float(i)), uv.y)).rgb * weights[i];
	}

	// Color lerp between blur and sharp.
	FragColor = vec4(mix(sharp, blur, depth), 1.0f);
}