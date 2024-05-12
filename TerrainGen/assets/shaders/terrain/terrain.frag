#version 330 core

out vec4 FragColor;

in VS_OUT {
    float vHeight;
    vec2 vTexCoord;
} fs_in;

const float EPSILON = 1e-4;
const int MAX_NUM_OF_COLORS = 8;
uniform int uNumOfColors;

// vHeight > biomeStartHeight --> apply biomeColor
uniform float uBiomeStartHeight[MAX_NUM_OF_COLORS];
uniform float uBiomeBlends[MAX_NUM_OF_COLORS];
uniform vec3 uBiomeColors[MAX_NUM_OF_COLORS];

uniform float uMinHeight;
uniform float uMaxHeight;

uniform sampler2D uGrass;
uniform sampler2D uSnow;

float inverseLerp(float a, float b, float value) {
    return clamp((value - a) / (b - a), 0.0, 1.0);
}

void main() {

    float heightPercent = inverseLerp(uMinHeight, uMaxHeight, fs_in.vHeight);
    // initially as gray
    vec3 color = vec3(heightPercent);

    for(int i = 0; i < uNumOfColors; i++) {
        //float drawStrength = clamp(sign(heightPercent - uBiomeStartHeight[i]), 0.0, 1.0);

        // blending the colors
        float drawStrength = inverseLerp(-uBiomeBlends[i] / 2 - EPSILON, uBiomeBlends[i] / 2, heightPercent - uBiomeStartHeight[i]);

        //color = color * (1 - drawStrength) + uBiomeColors[i] * drawStrength;
        // lerp
        color = mix(color, uBiomeColors[i], drawStrength);

        FragColor = vec4(color, 1.0);
    }

}
