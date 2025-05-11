#version 300 es
precision highp float;

uniform sampler2D Tex;

in vec2 UV;
flat in float brightness;
in float normBrightness;
in vec2 UVCenter;

layout (location = 0) out vec4 FragColor;

void main()
{	
    float highlightAlpha = smoothstep(1.0f, 1.2f, brightness);

    float brightnessMix = mix(normBrightness, 1.5f, highlightAlpha);

    vec4 texColor = texture(Tex, UV);

    FragColor = vec4(texColor.rgb * brightnessMix, texColor.a);
}