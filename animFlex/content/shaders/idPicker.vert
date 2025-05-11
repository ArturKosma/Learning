#version 300 es
precision highp float;

uniform uvec2 uObjectID;

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
layout (location = 2) in vec2 aUV;
layout (location = 3) in uint aFaceID;

flat out uvec4 UniqueID;

void main()
{
	UniqueID = uvec4(uObjectID.r, uObjectID.g, aFaceID, 1.0f);
	gl_Position = projection * view * modelTransform * vec4(aPos, 1.0f);
}