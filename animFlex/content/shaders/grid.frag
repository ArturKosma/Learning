#version 300 es
precision highp float;

in vec2 UV;
in float CellNum;

layout (location = 0) out vec4 FragColor;

float LinearizeDepth(float d, float zNear, float zFar)
{
	return (2.0f * zNear * zFar) / (zFar + zNear - (d * 2.0f - 1.0f) * (zFar - zNear));
}

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
	line = 0.0f;

	float depth = LinearizeDepth(gl_FragCoord.z, 0.1f, 100.0f) / 100.0f;

	// Sum the horizontal and vertical lines.
	// Clamp the brightness.
	float lineMask = clamp(line, 0.0f, 0.7f);

	// Define base quad color.
	vec4 baseColor = vec4(0.254f, 0.336f, 0.363f, 1.0f);

	// Final color.
	float lineColor = -0.22f * lineMask;

	//FragColor = vec4(vec3(depth), 1.0f);


	FragColor = baseColor + lineColor;
}