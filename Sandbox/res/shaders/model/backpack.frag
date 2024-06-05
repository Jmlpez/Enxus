#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1; // texture main color 
    sampler2D texture_specular1;

    // exercise
    float shininess; // radius of the specular highlitght
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

uniform Material material;
uniform DirLight uDirLight;
uniform vec3 uCameraPos;
uniform bool uFlipsUVs;

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;

vec2 FlipTextureUVs(vec2 texCoords, bool flip);

void main() {

    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(uCameraPos - vFragPos);

    // vTexCoord = FlipTextureUVs(uFlipsUVs);

    // phase 1: Directional lighting
    vec4 result = CalcDirLight(uDirLight, normal, viewDir);

    FragColor = result;
    //FragColor = texture2D(material.texture_diffuse1, vTexCoord);
}

vec2 FlipTextureUVs(vec2 texCoords, bool flip) {
    if(flip) {
        return vec2(texCoords.x, 1 - texCoords.y);
    }
    return texCoords;
}

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {

    vec3 lightDir = normalize(-light.direction);

    float diff = max(0.0, dot(lightDir, normal));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec2 coords = FlipTextureUVs(vTexCoord, uFlipsUVs);

    vec4 ambient = vec4(light.ambient, 1.0) * vec4(texture2D(material.texture_diffuse1, coords));
    vec4 diffuse = vec4(light.diffuse, 1.0) * diff * vec4(texture2D(material.texture_diffuse1, coords));

    vec4 specularTextureFulLColor = vec4(texture2D(material.texture_specular1, coords));
    // specularTextureFulLColor.y = specularTextureFulLColor.x;
    // specularTextureFulLColor.z = specularTextureFulLColor.x;
    //vec3 specular = light.specular * spec * specularTextureFulLColor;
    vec4 specular = vec4(light.specular, 1.0) * spec * specularTextureFulLColor;

    return ambient + diffuse + specular;
}
