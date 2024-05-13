#version 330 core

out vec4 FragColor;

in VS_OUT {
    float vHeight;
    vec3 vVertexPos;
    vec2 vTexCoord;
    vec3 vNormal;
    vec3 vFragPos;
} fs_in;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
uniform DirLight uDirLight;

const float EPSILON = 1e-4;
const int MAX_NUM_OF_COLORS = 8;
uniform int uNumOfColors;

// vHeight > biomeStartHeight --> apply biomeColor
uniform float uBiomeStartHeight[MAX_NUM_OF_COLORS];
uniform float uBiomeBlends[MAX_NUM_OF_COLORS];
uniform vec3 uBiomeColors[MAX_NUM_OF_COLORS];

uniform float uMinHeight;
uniform float uMaxHeight;

uniform float uTexScale;

uniform sampler2D uGrass;
//uniform sampler2D uSnow;

uniform vec3 uCameraPos;

float inverseLerp(float a, float b, float value);

void main() {

    float heightPercent = inverseLerp(uMinHeight, uMaxHeight, fs_in.vHeight);
    // initially as gray
    vec3 color = vec3(heightPercent);

    for(int i = 0; i < uNumOfColors; i++) {
        // blending the colors
        float drawStrength = inverseLerp(-uBiomeBlends[i] / 2 - EPSILON, uBiomeBlends[i] / 2, heightPercent - uBiomeStartHeight[i]);
        //color = color * (1 - drawStrength) + uBiomeColors[i] * drawStrength;
        // lerp
        color = mix(color, uBiomeColors[i], drawStrength);
    }

    vec3 normal = normalize(fs_in.vNormal);
    vec3 viewDir = normalize(uCameraPos - fs_in.vFragPos);

    vec3 lighting = CalcDirLight(uDirLight, normal, viewDir);
    //vec3 result = color;

    //FragColor = vec4(result, 1.0);

    vec3 scaledWorldPos = fs_in.vVertexPos / uTexScale;

    vec3 blendAxes = abs(fs_in.vNormal);
    blendAxes /= blendAxes.x + blendAxes.y + blendAxes.z;

    vec4 xProjection = texture2D(uGrass, scaledWorldPos.yz) * blendAxes.x;
    vec4 yProjection = texture2D(uGrass, scaledWorldPos.xz) * blendAxes.y;
    vec4 zProjection = texture2D(uGrass, scaledWorldPos.xy) * blendAxes.z;

    FragColor = (xProjection + yProjection + zProjection) * vec4(lighting, 1);
}

float inverseLerp(float a, float b, float value) {
    return clamp((value - a) / (b - a), 0.0, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {

    vec3 lightDir = normalize(-light.direction);

    float diff = max(0.0, dot(lightDir, normal));

    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);

    //vec3 ambient = light.ambient * vec3(texture2D(uObjectMaterial.diffuse, vTexCoord));
    //vec3 diffuse = light.diffuse * diff * vec3(texture2D(uObjectMaterial.diffuse, vTexCoord));
    //vec3 specular = light.specular * spec * vec3(texture2D(uObjectMaterial.specular, vTexCoord));

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    //vec3 specular = light.specular * spec;

    return ambient + diffuse;
}