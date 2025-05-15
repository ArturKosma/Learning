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

layout (std140) uniform JointMatrices
{
	mat4 jointMat[200]; // Hardcoded number of joints. #todo variable this.
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in uvec4 aJointNum;
layout (location = 4) in vec4 aJointWeight;

out vec4 Pos;
out vec3 Normal;
out vec2 UV;

void main()
{
	mat3 normalMatrix = mat3(transpose(inverse(modelTransform))); // This is expensive. #todo pass as uniform?
	vec3 worldNormal = normalize(normalMatrix * aNormal);

	mat4 skinMat = 
	aJointWeight.x * jointMat[aJointNum.x] +
	aJointWeight.y * jointMat[aJointNum.y] +
	aJointWeight.z * jointMat[aJointNum.z] +
	aJointWeight.w * jointMat[aJointNum.w];

	vec4 worldPos = modelTransform * skinMat * vec4(aPos, 1.0f);

	Pos = worldPos;
	Normal = worldNormal;
	UV = aUV;

	gl_Position = projection * view * worldPos;
}