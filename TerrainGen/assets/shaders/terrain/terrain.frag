#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 vVertexPos;
    vec3 vNormal;
    vec3 vBlendAxes;
    vec3 vFragPos;
    vec4 vFragPosLightSpace;
} fs_in;

struct DirLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
uniform DirLight uDirLight;

const float EPSILON = 1e-7;
const int MAX_NUM_OF_LAYERS = 8;
uniform int uNumOfLayers;

// vVertexPos.y > biomeStartHeight --> apply biomeColor
uniform float uBlendBoundaries[MAX_NUM_OF_LAYERS];
uniform float uColorStrength[MAX_NUM_OF_LAYERS];
uniform float uSlopeHeightBegin[MAX_NUM_OF_LAYERS];
uniform float uSlopeHeightEnd[MAX_NUM_OF_LAYERS];
uniform float uSlopeThreshold[MAX_NUM_OF_LAYERS];
uniform float uBlendSlope[MAX_NUM_OF_LAYERS];
uniform float uBlendLayer[MAX_NUM_OF_LAYERS];
uniform vec3 uColors[MAX_NUM_OF_LAYERS];
uniform bool uTextureUsed[MAX_NUM_OF_LAYERS];

uniform float uMinHeight;
uniform float uMaxHeight;

uniform bool uUseShadows;
uniform sampler2D uShadowMap; // slot 0
uniform sampler2D uTerrainTextures[8]; // slot from 1-8
uniform float uTexturesScale[8];

uniform vec3 uCameraPos;

float InverseLerp(float a, float b, float value);
vec4 Triplanar(vec3 vertexPos, float scale, vec3 blendAxes, int textureIndex);
float CalculateShadow(vec4 fragPosLightSpace, float bias);

void main() {

    float heightPercent = InverseLerp(uMinHeight, uMaxHeight, fs_in.vVertexPos.y);
    vec3 defaultColor = vec3(heightPercent);
    // initially as gray
    vec3 albedoColor = defaultColor;

    float slope = 1.0 - fs_in.vNormal.y;
    for(int i = 0; i < uNumOfLayers; i++) {

        vec3 baseColor = uColors[i] * uColorStrength[i];
        vec4 textureColor = vec4(0.0);
        if(uTextureUsed[i]) {
            textureColor = Triplanar(fs_in.vVertexPos, uTexturesScale[i], fs_in.vBlendAxes, i) * (1.0 - uColorStrength[i]);
        }
        vec3 layerColor = baseColor + textureColor.xyz;

        // Calculate the min value for the slope  to be interpolated based on the blending factor
        float minSlopeThreshold = uSlopeThreshold[i] * (1.0 - uBlendSlope[i]);
        float slopeWeight = 1.0 - InverseLerp(minSlopeThreshold - EPSILON, uSlopeThreshold[i], slope);

        vec3 slopeColor = mix(albedoColor, layerColor, slopeWeight * uBlendLayer[i]);

        float drawStrength = InverseLerp(-uBlendBoundaries[i] / 2 - EPSILON, uBlendBoundaries[i] / 2, heightPercent - uSlopeHeightEnd[i]);
        vec3 upHeightColor = mix(slopeColor, albedoColor, drawStrength);

        drawStrength = InverseLerp(-uBlendBoundaries[i] / 2 - EPSILON, uBlendBoundaries[i] / 2, heightPercent - uSlopeHeightBegin[i]);
        albedoColor = mix(albedoColor, upHeightColor, drawStrength);
    }

    vec3 normal = normalize(fs_in.vNormal);
    vec3 viewDir = normalize(uCameraPos - fs_in.vFragPos);

    vec3 lighting = CalcDirLight(uDirLight, normal, viewDir);
    vec3 result = lighting * albedoColor;

    FragColor = vec4(result, 1.0);

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

    vec3 lightDir = normalize(light.position - fs_in.vFragPos);
    float diff = max(0.0, dot(lightDir, normal));

    //vec3 reflectDir = reflect(-lightDir, normal);
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256);
    float specularStrength = 0.5;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec3 ambient = light.ambient;
    vec3 diffuse = light.diffuse * diff;
    vec3 specular = light.specular * spec * specularStrength;

    // add shadow calculation
    // In case of this terrain this value for bias works fine
    float shadow = 0;
    if(uUseShadows) {
        //float bias = mix(0.005, 0.0, diff);
        float bias = 0.025;
        shadow = CalculateShadow(fs_in.vFragPosLightSpace, bias);
    }

    return ambient + ((1 - shadow) * (diffuse + specular));
}

float CalculateShadow(vec4 fragPosLightSpace, float bias) {

    // using orthographic projection this step is not really necessary, but I will leave it here 
    vec3 uvCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // Transform from [-1, 1] to [0, 1]
    uvCoords = uvCoords * 0.5 + 0.5;

    float currentDepth = uvCoords.z;

    float shadow = 0;

    vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);
    for(int y = -1; y <= 1; y++) {
        for(int x = -1; x <= 1; x++) {
            float pcfDepth = texture2D(uShadowMap, uvCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    if(currentDepth > 1.0)
        shadow = 0;

    return shadow;

}
