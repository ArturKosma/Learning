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
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;

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
	// Cache near/far planes.
	float near = renderProperties[0][2];
	float far = renderProperties[0][3];

	// Scale the positions of grid quad vertices.
	float cellNum = far * 3.0f;
	vec3 posScaled = aPos * cellNum;

	// Find yaw dot to vertex.
	vec3 cameraPos = cameraTransform[3].xyz;
	cameraPos.y = 0.0f;
	vec3 dirToVertex = normalize(posScaled - aPos);
	vec3 cameraFwd = normalize(vec3(cameraTransform[2].xyz));
	vec3 cameraFwdFlat = -normalize(vec3(cameraFwd.x, 0.0f, cameraFwd.z));
	float dotToVertex = dot(cameraFwdFlat, dirToVertex);
	dotToVertex = Remap(dotToVertex, 1.0f, 0.75f, 0.0f, 1.0f);

	// Offset height of vertex depending on distance and dot.
	// This creates the horizon globe effect.
	float dist = length(vec2(posScaled.xz));
	float distScaleFactor = Remap(dist, 170.0f, 300.0f, 0.0f, 1.0f);
	float heightOffset = distScaleFactor * -50.0f;
	heightOffset += (dotToVertex * 30.0f * distScaleFactor);
	heightOffset = 0.0f;

	// Move the whole grid with camera with.
	float gridStepSize = 1.0f; 
	vec2 snapOffset = round(cameraTransform[3].xz / gridStepSize) * gridStepSize;

	float xPos = posScaled.x + snapOffset.x;
	float zPos = posScaled.z + snapOffset.y;
	vec3 posOffset = vec3(xPos, posScaled.y + heightOffset, zPos);

	// Pass information further.
	UV = aUV;
	CellNum = cellNum;
	WorldPosition = vec4(posOffset, 1.0f).xyz;
	gl_Position = projection * view * vec4(posOffset, 1.0f);
}