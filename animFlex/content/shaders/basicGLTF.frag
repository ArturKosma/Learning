#version 300 es
precision highp float;

layout (std140) uniform Camera
{
	mat4 cameraTransform;
};

uniform sampler2D Tex;

in vec3 Normal;
in vec2 UV;

layout (location = 0) out vec4 FragColor;

void main()
{	
	vec3 cameraFwd = normalize(vec3(cameraTransform[2].xyz));
	float lightAngle = abs(dot(normalize(Normal), cameraFwd));

	FragColor = texture(Tex, UV) * vec4(vec3(lightAngle), 1.0f);
}