#version 300 es
precision highp float;

layout (std140) uniform ViewProjection
{
	mat4 view;
	mat4 projection;
};

layout (std140) uniform Model
{
	mat4 modelTransform;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 7) in vec3 aColor;

out vec4 Pos;
out vec3 Normal;
out vec2 UV;
out vec3 Color;

void main()
{
	vec4 worldPos = modelTransform * vec4(aPos, 1.0f);

	mat3 normalMatrix = mat3(transpose(inverse(modelTransform))); // This is expensive. #todo pass as uniform?
	vec3 worldNormal = normalize(normalMatrix * aNormal);

	Pos = worldPos;
	Normal = worldNormal;
	UV = aUV;
	Color = aColor;

	gl_Position = projection * view * worldPos;
}