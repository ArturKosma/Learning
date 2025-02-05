#version 300 es
precision highp float;

layout (std140) uniform ViewProjection
{
	mat4 view;
	mat4 projection;
};

layout (std140) uniform Model
{
	mat4 model;
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

void main()
{
	// Define number of grid cells in a row.
	// Scale the positions of grid quad vertices.
	float cellNum = 500.0f;
	vec3 posScaled = aPos * cellNum;

	// Move the whole grid with camera with.
	float gridStepSize = 1.0f; 
	vec2 snapOffset = round(cameraTransform[3].xz / gridStepSize) * gridStepSize;
	snapOffset = vec2(0.0f);
	float xPos = posScaled.x + snapOffset.x;
	float zPos = posScaled.z + snapOffset.y;
	vec3 posOffset = vec3(xPos, posScaled.y, zPos);

	// Pass information further.
	UV = aUV;
	CellNum = cellNum;
	gl_Position = projection * view * vec4(posOffset, 1.0);
}