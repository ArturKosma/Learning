#version 300 es
precision mediump float;

layout (std140) uniform UITransform
{
	mat4 uiTransform;
};

layout (std140) uniform OrthoProjection
{
    mat4 orthoProjection;
};

layout (std140) uniform ViewRotation
{
	mat4 viewRotation;
};

uniform int aGlyphID;
const vec3 glyphOffsets[3] = vec3[]
    (
        vec3(0.18f, 0.03f, 0.0f),
        vec3(0.03f, 0.18f, 0.0f),
        vec3(0.0f, 0.03f, 0.18f)
    );

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 UV;
flat out int glyphID;

void main()
{
    vec4 center = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 rotatedCenter = viewRotation * (center + vec4(glyphOffsets[aGlyphID], 0.0f));
    vec4 worldPosition = rotatedCenter + vec4(aPos.xy, 0.0, 0.0); // Always facing forward.

    gl_Position = uiTransform * orthoProjection * worldPosition;
	UV = aUV;
    glyphID = aGlyphID;
}