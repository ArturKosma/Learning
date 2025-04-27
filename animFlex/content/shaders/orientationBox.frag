#version 300 es
precision mediump float;

uniform sampler2D Tex;

in vec2 UV;
flat in float brightness;
in float normBrightness;
in float distBrightness;
in vec2 UVCenter;

layout (location = 0) out vec4 FragColor;

void main()
{	
    float highlightAlpha = smoothstep(1.0f, 1.2f, brightness);
    highlightAlpha *= smoothstep(0.0f, 0.8f, (1.0f - (length(UV - UVCenter) * 5.0f))); // Little circle of highlight instead of full face.

    float brightnessMix = mix(normBrightness * distBrightness, brightness / 1.4f, highlightAlpha);

    vec4 texColor = texture(Tex, UV);
    vec3 yellowColor = mix(vec3(0.0f, 0.0f, 0.0f), vec3(0.18f, 0.18f, 0.0f), highlightAlpha);

    FragColor = vec4((texColor.rgb + yellowColor) * brightnessMix, texColor.a);
}