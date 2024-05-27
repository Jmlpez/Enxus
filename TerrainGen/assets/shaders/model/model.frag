#version 330 core

out vec4 FragColor;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
uniform DirLight uDirLight;

in VS_OUT {
    vec2 vTexCoord;
    vec3 vNormal;
    vec3 vFragPos;
} fs_in;

// Example
struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;
};

uniform Material uMaterial;

uniform vec3 uCameraPos;

void main() {

    vec3 normal = normalize(fs_in.vNormal);
    vec3 viewDir = normalize(uCameraPos - fs_in.vFragPos);

    vec4 result = CalcDirLight(uDirLight, normal, viewDir);

    FragColor = result;

}

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {

    vec3 lightDir = normalize(-light.direction);

    float diff = max(0.0, dot(lightDir, normal));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 256.0);

    //vec3 ambient = light.ambient * vec3(texture2D(uObjectMaterial.diffuse, vTexCoord));
    //vec3 diffuse = light.diffuse * diff * vec3(texture2D(uObjectMaterial.diffuse, vTexCoord));
    //vec3 specular = light.specular * spec * vec3(texture2D(uObjectMaterial.specular, vTexCoord));

    vec4 diffuseColor = vec4(texture2D(uMaterial.diffuseMap, fs_in.vTexCoord));
    vec4 specularColor = vec4(texture2D(uMaterial.specularMap, fs_in.vTexCoord));

    vec4 ambient = vec4(light.ambient, 1.0) * diffuseColor;
    vec4 diffuse = vec4(light.diffuse, 1.0) * diff * diffuseColor;
    vec4 specular = vec4(light.specular, 1.0) * spec * specularColor;

    return ambient + diffuse + specular;
}