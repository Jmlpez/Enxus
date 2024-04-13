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
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

uniform Material material;
uniform DirLight uDirLight;
uniform vec3 uCameraPos;

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;

void main() {

    vec3 normal = normalize(vNormal);
    vec3 viewDir = normalize(uCameraPos - vFragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(uDirLight, normal, viewDir);

    FragColor = vec4(result, 1.0);
    //FragColor = texture2D(material.texture_diffuse1, vTexCoord);

}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {

    vec3 lightDir = normalize(-light.direction);

    float diff = max(0.0, dot(lightDir, normal));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture2D(material.texture_diffuse1, vTexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture2D(material.texture_diffuse1, vTexCoord));

    vec3 specularTextureFulLColor = vec3(texture2D(material.texture_specular1, vTexCoord));
    // specularTextureFulLColor.y = specularTextureFulLColor.x;
    // specularTextureFulLColor.z = specularTextureFulLColor.x;
    //vec3 specular = light.specular * spec * specularTextureFulLColor;
    vec3 specular = light.specular * spec * specularTextureFulLColor;

    return ambient + diffuse + specular;
}
