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

layout (std140) uniform RenderProperties
{
	mat4 renderProperties;
};

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aUV;

out vec2 UV;
out float GridSize;
out float CellSize;
out vec3 WorldPosition;

float Remap(float value, float inMin, float inMax, float outMin, float outMax)
{
	float mappedValue = (value - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;

	return clamp(mappedValue, min(outMin, outMax), max(outMin, outMax));
}

void main()
{
	float gridSize = 20000.0f * 2.0f;
	float cellSize = 100.0f;

	vec2 snapOffset = round(cameraTransform[3].xz / cellSize) * cellSize;

	float xPos = (aPos.x * gridSize) + snapOffset.x;
	float zPos = (aPos.z * gridSize) + snapOffset.y;
	vec3 posOffset = vec3(xPos, aPos.y, zPos);

	// Pass information further.
	UV = aUV;
	WorldPosition = posOffset;
	GridSize = gridSize;
	CellSize = cellSize;
	gl_Position = projection * view * vec4(posOffset, 1.0f);
}