#version 300 es
precision mediump float;

uniform sampler2D Tex;

in vec2 UV;
flat in float brightness;
in float normBrightness;
in float distBrightness;

layout (location = 0) out vec4 FragColor;

void main()
{	
    float highlightAlpha = smoothstep(1.0f, 1.2f, brightness);
    float brightnessMix = mix(normBrightness * distBrightness, brightness, highlightAlpha);

    vec4 texColor = texture(Tex, UV);

    FragColor = vec4(texColor.rgb * brightnessMix, texColor.a);
}