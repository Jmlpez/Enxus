#version 330 core

out vec4 FragColor;

in VS_OUT {
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

// vVertexPos.y > biomeStartHeight --> apply biomeColor
uniform float uBiomeStartHeight[MAX_NUM_OF_COLORS];
uniform float uBiomeBlends[MAX_NUM_OF_COLORS];
uniform float uBiomeColorStrength[MAX_NUM_OF_COLORS];
uniform vec3 uBiomeColors[MAX_NUM_OF_COLORS];
uniform bool uBiomeTextureUsed[MAX_NUM_OF_COLORS];

uniform float uMinHeight;
uniform float uMaxHeight;

//uniform sampler2D uSnow;
uniform sampler2D uTerrainTextures[8];
uniform float uTexturesScale[8];
//uniform sampler2D uGrass;

uniform vec3 uCameraPos;

float InverseLerp(float a, float b, float value);
vec4 Triplanar(vec3 vertexPos, float scale, vec3 blendAxes, int textureIndex);

void main() {

    float heightPercent = InverseLerp(uMinHeight, uMaxHeight, fs_in.vVertexPos.y);
    // initially as gray
    vec3 albedoColor = vec3(heightPercent);

    // Blend axes for triplanar mappgin
    vec3 blendAxes = abs(fs_in.vNormal);
    blendAxes /= blendAxes.x + blendAxes.y + blendAxes.z;

    for(int i = 0; i < uNumOfColors; i++) {
        // blending the colors
        float drawStrength = InverseLerp(-uBiomeBlends[i] / 2 - EPSILON, uBiomeBlends[i] / 2, heightPercent - uBiomeStartHeight[i]);

        vec4 textureColor = vec4(0);

        vec3 baseColor = uBiomeColors[i] * uBiomeColorStrength[i];
        if(uBiomeTextureUsed[i]) {
            textureColor = Triplanar(fs_in.vVertexPos, uTexturesScale[i], blendAxes, i) * (1 - uBiomeColorStrength[i]);
        }

        vec3 color = textureColor.xyz + baseColor;

        albedoColor = mix(albedoColor, color, drawStrength);
    }

    vec3 normal = normalize(fs_in.vNormal);
    vec3 viewDir = normalize(uCameraPos - fs_in.vFragPos);

    vec3 lighting = CalcDirLight(uDirLight, normal, viewDir);
    vec3 result = albedoColor * lighting;

    FragColor = vec4(result, 1.0);

    // vec3 scaledWorldPos = fs_in.vVertexPos / uTexScale;

    // vec4 xProjection = texture2D(uTerrainTextures[0], scaledWorldPos.yz) * blendAxes.x;
    // vec4 yProjection = texture2D(uTerrainTextures[0], scaledWorldPos.xz) * blendAxes.y;
    // vec4 zProjection = texture2D(uTerrainTextures[0], scaledWorldPos.xy) * blendAxes.z;

}

float InverseLerp(float a, float b, float value) {
    return clamp((value - a) / (b - a), 0.0, 1.0);
}

vec4 Triplanar(vec3 vertexPos, float scale, vec3 blendAxes, int textureIndex) {
    vec3 scaledTexCoord = vertexPos / scale;

    vec4 textureYZ;
    vec4 textureXZ;
    vec4 textureXY;
    switch(textureIndex) {
        case 0:
            textureYZ = texture2D(uTerrainTextures[0], scaledTexCoord.yz);
            textureXZ = texture2D(uTerrainTextures[0], scaledTexCoord.xz);
            textureXY = texture2D(uTerrainTextures[0], scaledTexCoord.xy);
            break;
        case 1:
            textureYZ = texture2D(uTerrainTextures[1], scaledTexCoord.yz);
            textureXZ = texture2D(uTerrainTextures[1], scaledTexCoord.xz);
            textureXY = texture2D(uTerrainTextures[1], scaledTexCoord.xy);
            break;
        case 2:
            textureYZ = texture2D(uTerrainTextures[2], scaledTexCoord.yz);
            textureXZ = texture2D(uTerrainTextures[2], scaledTexCoord.xz);
            textureXY = texture2D(uTerrainTextures[2], scaledTexCoord.xy);
            break;
        case 3:
            textureYZ = texture2D(uTerrainTextures[3], scaledTexCoord.yz);
            textureXZ = texture2D(uTerrainTextures[3], scaledTexCoord.xz);
            textureXY = texture2D(uTerrainTextures[3], scaledTexCoord.xy);
            break;
        case 4:
            textureYZ = texture2D(uTerrainTextures[4], scaledTexCoord.yz);
            textureXZ = texture2D(uTerrainTextures[4], scaledTexCoord.xz);
            textureXY = texture2D(uTerrainTextures[4], scaledTexCoord.xy);
            break;
        case 5:
            textureYZ = texture2D(uTerrainTextures[5], scaledTexCoord.yz);
            textureXZ = texture2D(uTerrainTextures[5], scaledTexCoord.xz);
            textureXY = texture2D(uTerrainTextures[5], scaledTexCoord.xy);
            break;
        case 6:
            textureYZ = texture2D(uTerrainTextures[6], scaledTexCoord.yz);
            textureXZ = texture2D(uTerrainTextures[6], scaledTexCoord.xz);
            textureXY = texture2D(uTerrainTextures[6], scaledTexCoord.xy);
            break;
        case 7:
            textureYZ = texture2D(uTerrainTextures[7], scaledTexCoord.yz);
            textureXZ = texture2D(uTerrainTextures[7], scaledTexCoord.xz);
            textureXY = texture2D(uTerrainTextures[7], scaledTexCoord.xy);
            break;
    }

    vec4 xProjection = textureYZ * blendAxes.x;
    vec4 yProjection = textureXZ * blendAxes.y;
    vec4 zProjection = textureXY * blendAxes.z;

    return (xProjection + yProjection + zProjection);
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