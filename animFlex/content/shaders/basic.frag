#version 300 es
precision highp float;

uniform sampler2D Tex;

layout (std140) uniform Camera
{
	mat4 cameraTransform;
};

in vec4 Pos;
in vec2 UV;
in vec3 Normal;

layout (location = 0) out vec4 FragColor;

void main()
{	
	vec3 cameraWorldPos = cameraTransform[3].xyz;
	vec3 fragWorldPos = Pos.xyz;

    vec3 toFrag = normalize(fragWorldPos - cameraWorldPos);

	float lightAngle = max(abs(dot(normalize(Normal), toFrag)), 0.3f);

	FragColor = texture(Tex, UV) * vec4(vec3(lightAngle), 1.0f);
}