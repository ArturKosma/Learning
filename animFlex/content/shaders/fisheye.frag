#version 300 es
precision mediump float;

uniform sampler2D u_ColorTex;
uniform sampler2D u_DepthTex;

layout (std140) uniform RenderProperties
{
	mat4 renderProperties;
};

layout (location = 0) out vec4 FragColor;

float LinearizeDepth(float d, float zNear, float zFar)
{
	return (2.0f * zNear) / (zFar + zNear - d * (zFar - zNear));
}

// Given a vec2 in <-1.0f, 1.0f>, generate a texture coord in <0.0f, 1.0f>.
vec2 BarrelDistortion(vec2 p)
{
	const float barrelPowerMaxOffset = 0.2f;

	float theta  = atan(p.y, p.x);
    float radius = length(p);
    radius = pow(radius, 1.0f + barrelPowerMaxOffset);
    p.x = radius * cos(theta);
    p.y = radius * sin(theta);
    return 0.5 * (p + 1.0);
}

void main()
{	
	vec2 res = vec2(renderProperties[0][0], renderProperties[0][1]);

	// Get screen space UV.
    vec2 uv = gl_FragCoord.xy / res;

	// Get <-1.0f, 1.0f> UV space.
	uv = (uv - 0.5f) * 2.0f;

	vec4 screen = texture(u_ColorTex, BarrelDistortion(uv));
	vec4 finalColor = vec4(vec3(screen.xyz), 1.0f);

	FragColor = finalColor;
}