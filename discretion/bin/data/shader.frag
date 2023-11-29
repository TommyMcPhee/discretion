#version 150


uniform sampler2DRect tex0;
in vec2 texCoordVarying;
out vec4 outputColor;
uniform float oldMix;
uniform float feedback;
uniform vec2 window;
uniform vec3 feedbackDistortion;
uniform vec4 power;
uniform vec4 feedbackPower;
uniform mat3 newX;
uniform mat3 newY;
uniform mat3 oldX;
uniform mat3 oldY;
uniform mat3 distortX;
uniform mat3 distortY;
uniform mat3 displace;
uniform mat3 amplitude;

vec3 synthesize(mat2x3 phase, mat2x3 frequency, mat2x3 power, vec2 twoPiNormalized){
        vec3 output = vec3(0.0);
        for(int a = 0; a < 3; a++){
            float component = (pow(abs(sin(twoPiNormalized.x * frequency[0][a] + phase[0][a])), power[0][a]) * sign(twoPiNormalized.x * frequency[0][a] + phase[0][a]) * 0.25 + 0.25) + (pow(abs(sin(twoPiNormalized.y * frequency[1][a] + phase[1][a])), power[1][a]) * sign(twoPiNormalized.y * frequency[1][a] + phase[1][a]) * 0.25 + 0.25);
            output[a] = component;
        }
        return output;
}

void main()
{
    vec2 normalized = gl_FragCoord.xy / window;
    vec2 twoPiNormalized = normalized * 6.28318531;
    mat2x3 newPhase = mat2x3(newX[0], newY[0]);
    mat2x3 newFrequency = mat2x3(newX[1], newY[1]);
    mat2x3 newPower = mat2x3(newX[2], newY[2]);
    mat2x3 oldPhase = mat2x3(oldX[0], oldY[0]);
    mat2x3 oldFrequency = mat2x3(oldX[1], oldY[1]);
    mat2x3 oldPower = mat2x3(oldX[2], oldY[2]);
    mat2x3 distortPhase = mat2x3(distortX[0], distortY[0]);
    mat2x3 distortFrequency = mat2x3(distortX[1], distortY[1]);
    mat2x3 distortPower = mat2x3(distortX[2], distortY[2]);
    vec3 new = synthesize(newPhase, newFrequency, newPower, twoPiNormalized) * amplitude[0];
    vec3 old = synthesize(oldPhase, oldFrequency, oldPower, twoPiNormalized) * amplitude[1];
    vec3 distort = synthesize(distortPhase, distortFrequency, distortPower, twoPiNormalized) * amplitude[2];
    vec4 feedbackColor = texture2DRect(tex0, (texCoordVarying + (feedbackDistortion.xy * window)) * feedbackDistortion.z);
    vec3 adjustedFeedbackColor = vec3(pow(feedbackColor.r, feedbackPower.r * feedbackPower.a), pow(feedbackColor.g, feedbackPower.g * feedbackPower.a), pow(feedbackColor.b, feedbackPower.b * feedbackPower.a));
    vec4 oldColorR = texture2DRect(tex0, (texCoordVarying + (distort.r + displace[0].xy) * window) * displace[0].z);
    vec4 oldColorG = texture2DRect(tex0, (texCoordVarying + (distort.g + displace[1].xy) * window) * displace[1].z);
    vec4 oldColorB = texture2DRect(tex0, (texCoordVarying + (distort.b + displace[2].xy) * window) * displace[2].z);
    vec3 oldColor = vec3(oldColorR.r, oldColorG.g, oldColorB.b) * old;
    vec3 newColor = new * (1.0 - old);
    vec3 color = mix(oldColor, newColor, oldMix);
    vec3 adjustedColor = vec3(pow(color.r, power.r * power.a), pow(color.g, power.g * power.a), pow(color.b, power.b * power.a));
    outputColor = vec4(1.0, 0.0, 0.0, 1.0);
}