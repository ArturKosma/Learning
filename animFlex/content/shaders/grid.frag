#version 300 es
precision highp float;

in vec2 UV;
in float CellNum;

layout (location = 0) out vec4 FragColor;

void main()
{	
	vec2 scaledUV = UV * CellNum;
	vec2 gridUV = fract(scaledUV);

	float k = 40.0f;

	float oneUp_x = 1.0f / exp(-k * (2.0f * gridUV.x - 2.0f));
	float zeroUp_x = 1.0f / exp(-k * (-2.0f * gridUV.x));
	float oneUp_y = 1.0f / exp(-k * (2.0f * gridUV.y - 2.0f));
	float zeroUp_y = 1.0f / exp(-k * (-2.0f * gridUV.y));

	float gridMask = oneUp_x + zeroUp_x + oneUp_y + zeroUp_y;

	float lineWidth = 0.02f;
	float smoothStepGridMask = smoothstep(0.5f - lineWidth, 0.5f + lineWidth, gridMask);

	//float aa = fwidth(smoothStepGridMask);
    //float smoothStepGridMask = smoothstep(0.5f - aa, 0.5f + aa, gridMask);

	FragColor = vec4(vec3(smoothStepGridMask), 1.0f);
	//FragColor = vec4(0.77f - brightnessOffset);
}