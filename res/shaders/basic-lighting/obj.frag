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

struct Light {
    // vec3 direction; // use direction instead of position for directional light
    vec3 position;
    vec3 direction;
    float innerCutOffAngle;
    float outerCutOffAngle;

    vec3 ambient; // ambient light (usually a low value)
    vec3 diffuse; // light color
    vec3 specular; // specular light color (usually as bright as it can) 

    // for attenuation
    float constant;
    float linear;
    float quadratic;
};

uniform Material uObjectMaterial;
uniform Light uLight;

//uniform vec3 u_LightColor;
//uniform vec3 u_LightPos;
uniform vec3 uCameraPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 vTexCoord;

void main() {

    // ambient color
    vec3 ambient = uLight.ambient * vec3(texture2D(uObjectMaterial.diffuse, vTexCoord));

    // diffuse color
    vec3 normal = normalize(Normal);
    //vec3 lightDir = normalize(-uLight.direction);
    vec3 lightDir = normalize(uLight.position - FragPos);
    float diff = max(0.0, dot(lightDir, normal));
    vec3 diffuse = uLight.diffuse * diff * vec3(texture2D(uObjectMaterial.diffuse, vTexCoord));

    // specular color
    vec3 viewDir = normalize(uCameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uObjectMaterial.shininess);

    vec3 specular = uLight.specular * spec * vec3(texture2D(uObjectMaterial.specular, vTexCoord));

    // emission color
    // vec3 emissionColor = uObjectMaterial.emission.color * uObjectMaterial.emission.strength;
    // vec3 emission = vec3(texture2D(uObjectMaterial.emission.texture, vTexCoord)) * emissionColor;

    // attenuation 1/(A*d^2 + B*d + C)
    float distance = length(uLight.position - FragPos);
    float attenuation = 1.0 / ((distance * distance) * uLight.quadratic + distance * uLight.linear + uLight.constant);

    // attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    float theta = dot(lightDir, -uLight.direction);
    float epsilon = uLight.innerCutOffAngle - uLight.outerCutOffAngle;

    float intensity = clamp((theta - uLight.outerCutOffAngle) / epsilon, 0.0, 1.0);

    // ambient not included because its value would be 0 outside the cone

    diffuse *= intensity;
    specular *= intensity;

    vec3 result = diffuse + ambient + specular;

    FragColor = vec4(result, 1.0);
}