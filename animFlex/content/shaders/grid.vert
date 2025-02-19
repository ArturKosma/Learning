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
layout (location = 1) in vec2 aUV;

out vec2 UV;
out float CellNum;
out vec3 WorldPosition;

float Remap(float value, float inMin, float inMax, float outMin, float outMax)
{
	float mappedValue = (value - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;

	return clamp(mappedValue, min(outMin, outMax), max(outMin, outMax));
}

void main()
{
	// Move the whole grid with camera.
	float gridStepSize = 6.0f / 3.0f; 
	vec2 snapOffset = round(cameraTransform[3].xz / gridStepSize) * gridStepSize;

	float cellNum = 300.0f;

	float xPos = (aPos.x * cellNum) + snapOffset.x;
	float zPos = (aPos.z * cellNum) + snapOffset.y;
	vec3 posOffset = vec3(xPos, aPos.y, zPos);

	// Pass information further.
	UV = aUV;
	WorldPosition = posOffset;
	CellNum = cellNum * 2.0f; // We need to "fake" twice cell num, because the model of the grid is really 2x2, not 1x1.
	gl_Position = projection * view * vec4(posOffset, 1.0f);
}