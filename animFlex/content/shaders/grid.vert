#version 300 es
precision highp float;

layout (std140) uniform Matrices
{
	mat4 view;
	mat4 projection;
	mat4 modelTransform;
};

layout (std140) uniform Camera
{
	mat4 cameraTransform;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;

out vec2 UV;
out float CellNum;
out vec3 WorldPos;

void main()
{
	float cellNum = 100.0f;
	vec3 posScaled = aPos * cellNum;

	float gridStepSize = 2.0f; 
	vec2 snapOffset = round(cameraTransform[3].xz / gridStepSize) * gridStepSize;

	float xPos = posScaled.x + snapOffset.x;
	float zPos = posScaled.z + snapOffset.y;

	vec3 posOffset = vec3(xPos, posScaled.y, zPos);

	UV = aUV;
	CellNum = cellNum;

	gl_Position = projection * view * vec4(posOffset, 1.0);
}