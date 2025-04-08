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
const float weights[6] = float[](0.2270, 0.1946, 0.1216, 0.0541, 0.0162, 0.0044);

void main()
{	
	// Get resolution.
	vec2 res = vec2(renderProperties[0][0], renderProperties[0][1]);

	// Get screen space UV.
    vec2 uv = gl_FragCoord.xy / res;

	// Get texel height.
	float texelHeight = 1.0f / res.y;

	// Get depth modifier.
	// It controls the lerp between blur and clean image.
	float depth = texture(u_DepthTex, uv).x;
	depth = smoothstep(0.98f, 1.0f, depth);

	// Get center color.
	vec3 sharp = texture(u_ColorTex, uv).rgb;
	vec3 blur = texture(u_ColorTex, uv).rgb * weights[0];

	// Add kernel colors.
	for(int i = 1; i < 6; ++i)
	{
		blur += texture(u_ColorTex, vec2(uv.x, uv.y + texelHeight * float(i))).rgb * weights[i];
		blur += texture(u_ColorTex, vec2(uv.x, uv.y - texelHeight * float(i))).rgb * weights[i];
	}

	// Color lerp between blur and sharp.
	FragColor = vec4(mix(sharp, blur, depth), 1.0f);
}