#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec2 vTexCoord;
    vec3 vNormal;
    vec3 vFragPos;
    vec4 vFragPosLightSpace;
} fs_in;

// Example
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform Material material;

uniform bool uCastShadow;
uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform sampler2D uShadowDepthMap;

float ShadowCalculation(vec4 fragPosLightSpace, float bias);

void main() {

    vec2 invertTexCoord = vec2(fs_in.vTexCoord.x, 1 - fs_in.vTexCoord.y);
    vec3 color = texture(material.texture_diffuse1, invertTexCoord).rgb;
    vec3 normal = normalize(fs_in.vNormal);
    vec3 lightColor = vec3(0.5);

    // ambient
    vec3 ambient = 0.3 * lightColor;
    // diffuse
    vec3 lightDir = normalize(uLightPos - fs_in.vFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(uViewPos - fs_in.vFragPos);
    //vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    

    // calculate shadow
    float shadow = 0.0;
    if(uCastShadow) {
        float bias = mix(0.0, 0.005, dot(normal, lightDir));
        shadow = ShadowCalculation(fs_in.vFragPosLightSpace, bias);
    }
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;

    FragColor = vec4(lighting, 1.0);
}

float ShadowCalculation(vec4 fragPosLightSpace, float bias) {

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform from [-1, 1] to [0, 1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = 0;

    vec2 texelSize = 1.0 / textureSize(uShadowDepthMap, 0);

    for(int x = -1; x <= 1; x++) {
        for(int y = -1; y <= 1; y++) {
            float depth = texture2D(uShadowDepthMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > depth ? 1.0 : 0.0;
        }
    }
    // the average
    shadow /= 9.0;

    // if(projCoords.z > 1.0)
    //     shadow = 0;

    return shadow;
}