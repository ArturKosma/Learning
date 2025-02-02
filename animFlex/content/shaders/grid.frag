#version 300 es
precision highp float;

in vec2 UV;
in float CellNum;
in vec2 NearFarPlanes;

layout (location = 0) out vec4 FragColor;

float GridLineGeneration(vec2 in_fun_UV, float in_fun_cellNum)
{
	// Make repeating UV grid.
	vec2 scaledUV = in_fun_UV * in_fun_cellNum;
	vec2 gridUV = fract(scaledUV + 0.5f);

	// Get the minimum width of X and Y lines. 
	// Width becomes bigger with stronger UV changes (far distance, big angle).
	float scaledUVChangeMultiplier = 1.5f;
	float minLineWidthX = max(0.012f, fwidth(scaledUV.x) * scaledUVChangeMultiplier);  
	float minLineWidthY = max(0.012f, fwidth(scaledUV.y) * scaledUVChangeMultiplier);

	// Get the distance from the center of UV tile.
	float distX = abs(gridUV.x - 0.5f);
	float distY = abs(gridUV.y - 0.5f);

	// Smoothly transition between 0.0f and 1.0f for X and Y lines,
	// based on the pixel distance from center of UV tile.
	// Order is reversed, so <dist> being bigger than <minWidth> returns 0.0f.
	float lineX = smoothstep(minLineWidthX, 0.0f, distX);
	float lineY = smoothstep(minLineWidthY, 0.0f, distY);

	// Sum the lines.
	float line = clamp(lineX + lineY, 0.0f, 1.0f);

	return line;
}

void main()
{	
	// Generate brightness for the grid line.
	float line = GridLineGeneration(UV, CellNum);

	// Sum the horizontal and vertical lines.
	// Clamp the brightness.
	float lineMask = clamp(line, 0.0f, 0.7f);

	// Find depth mask to reduce grid lines brightness which are far away.
	float nearPlane = NearFarPlanes.x;
	float farPlane = NearFarPlanes.y;
	float depth = (2.0 * nearPlane) / (farPlane + nearPlane - gl_FragCoord.z * (farPlane - nearPlane));
	float depthClamped = clamp(depth, 0.0f, 0.9f);

	// Screen space vigniette.
	// @todo Push uniform with screen size.
	float screenUVx = gl_FragCoord.x / vec2(2560.0f, 1440.0f);
	float screenUVxDist = 1.0f - abs(0.5f - screenUVx);
	float vigniette = smoothstep(0.3f, 1.0f, screenUVxDist);

	// Find depth mask for the whole quad.
	float depthMaskQuad = (1.0f - smoothstep(0.0f, 0.9f, depthClamped)) * vigniette;

	// Find depth mask for the grid.
	float depthMaskGrid = (1.0f - smoothstep(0.0f, 0.6f, depthClamped)) * vigniette;

	// Get grid lines close to view.
	vec4 lineMaskDepth = vec4(vec3(lineMask * depthMaskGrid), 1.0f);

	// Define base quad color.
	vec4 baseColor = vec4(0.254f, 0.336f, 0.363f, 1.0f);

	// Get base color of the quad with depth.
	vec4 baseColorDepth = baseColor * depthMaskQuad;

	// Find the change of UV in screen space.
	// The more change - the more distorted the grid lines, so use that change to fade out the grid lines brightness.
	float changeMultiplier = 1000.0f;
	float changeX = fwidth(UV.x) * changeMultiplier;
	float changeY = fwidth(UV.y) * changeMultiplier;
	float change = clamp(1.0f - clamp(changeX + changeY, 0.0f, 1.0f), 0.1f, 0.8f);

	// Final color.
	float lineColor = -0.22f * lineMaskDepth;

	//FragColor = vec4(vec3(baseColorDepth), 1.0f);
	FragColor = (baseColorDepth + (lineColor * change));
}