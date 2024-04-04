#version 330 core

out vec4 FragColor;

struct Material {
    vec3 ambient; // ambient color (a.k.a object colour)
    vec3 diffuse;
    vec3 specular;

    float shininess; // radius of the specular highlitght
};

struct Light {
    vec3 position; // light source position
    vec3 ambient; // ambient light (usually a low value)
    vec3 diffuse; // light color
    vec3 specular; // specular light color (usually as bright as it can) 
};

uniform Material uObjectMaterial;
uniform Light uLight;

//uniform vec3 u_LightColor;
//uniform vec3 u_LightPos;
uniform vec3 uCameraPos;

in vec3 Normal;
in vec3 FragPos;

void main() {

    // ambient color
    //float ambientStrength = 0.1;
    //vec3 ambient = u_LightColor * ambientStrength;
    vec3 ambientColor = uLight.ambient * uObjectMaterial.ambient;

    // diffuse color
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(uLight.position - FragPos);
    float diff = max(0.0, dot(lightDir, normal));
    vec3 diffuse = uLight.diffuse * (uObjectMaterial.diffuse * diff);

    // specular color
    vec3 viewDir = normalize(uCameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uObjectMaterial.shininess);
    vec3 specular = uLight.specular * (uObjectMaterial.specular * spec);
    // result
    vec3 result = diffuse + ambientColor + specular;

    FragColor = vec4(result, 1.0);
}