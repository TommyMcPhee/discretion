#version 150

uniform sampler2DRect tex0;
in vec2 texCoordVarying;
out vec4 outputColor;
uniform vec2 window;
uniform vec3 rgb;

void main()
{
    vec2 normalized = gl_FragCoord.xy / window;
    vec2 twoPiNormalized = normalized * 6.28318531;

    outputColor = vec4(rgb.r, rgb.g, rgb.b, 1.0);
}