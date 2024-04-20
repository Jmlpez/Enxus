#version 330 core

out vec4 FragColor;

struct Emission {
    float strength;
    vec3 color;
    sampler2D texture;
};

struct Material {
    sampler2D diffuse; // texture main color 
    sampler2D specular;

    // exercise
    Emission emission;

    float shininess; // radius of the specular highlitght
};

struct PointLight {
    vec3 position;

    vec3 ambient; // ambient light (usually a low value)
    vec3 diffuse; // light color
    vec3 specular; // specular light color (usually as bright as it can) 

    // for attenuation
    float constant;
    float linear;
    float quadratic;
};
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

struct SpotLight {
    vec3 position;
    vec3 direction;
    float innerCutOffAngle;
    float outerCutOffAngle;

    vec3 diffuse;
    vec3 specular;
};
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

uniform Material uObjectMaterial;
uniform DirLight uDirLight;
uniform SpotLight uSpotLight;

#define NR_POINT_LIGHTS 4
uniform PointLight uPointLights[NR_POINT_LIGHTS];

uniform vec3 uCameraPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 vTexCoord;

void main() {

    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(uCameraPos - FragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(uDirLight, normal, viewDir);

    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++) {
        result += CalcPointLight(uPointLights[i], normal, FragPos, viewDir);
    }
    // phase 3: Spot light
    result += CalcSpotLight(uSpotLight, normal, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
    //FragColor = vec4(1.0, 0.0, 0.5, 1.0);

}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {

    vec3 lightDir = normalize(-light.direction);

    float diff = max(0.0, dot(lightDir, normal));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uObjectMaterial.shininess);

    vec3 ambient = light.ambient * vec3(texture2D(uObjectMaterial.diffuse, vTexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture2D(uObjectMaterial.diffuse, vTexCoord));
    vec3 specular = light.specular * spec * vec3(texture2D(uObjectMaterial.specular, vTexCoord));

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(0.0, dot(lightDir, normal));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uObjectMaterial.shininess);

    // attenuation 1/(A*d^2 + B*d + C)
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / ((distance * distance) * light.quadratic + distance * light.linear + light.constant);

    vec3 ambient = light.ambient * vec3(texture2D(uObjectMaterial.diffuse, vTexCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture2D(uObjectMaterial.diffuse, vTexCoord));
    vec3 specular = light.specular * spec * vec3(texture2D(uObjectMaterial.diffuse, vTexCoord));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(0.0, dot(lightDir, normal));

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uObjectMaterial.shininess);

    float theta = dot(lightDir, -light.direction);
    float epsilon = light.innerCutOffAngle - light.outerCutOffAngle;

    float intensity = clamp((theta - light.outerCutOffAngle) / epsilon, 0.0, 1.0);

    // ambient not included because its value would be 0 outside the cone

    vec3 diffuse = light.diffuse * diff * vec3(texture2D(uObjectMaterial.diffuse, vTexCoord));
    vec3 specular = light.specular * spec * vec3(texture2D(uObjectMaterial.diffuse, vTexCoord));

    diffuse *= intensity;
    specular *= intensity;

    return diffuse + specular;
}