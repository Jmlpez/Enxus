#version 330 core

out vec4 FragColor;

uniform vec3 u_LightColor;
uniform vec3 u_LightPos;
uniform vec3 u_ObjectColor;
uniform vec3 u_CameraPos;

in vec3 Normal;
in vec3 FragPos;

void main() {

    // ambient color
    float ambientStrength = 0.1;
    vec3 ambient = u_LightColor * ambientStrength;
    vec3 ambientColor = u_ObjectColor * ambient;

    // diffuse color
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(u_LightPos - FragPos);
    float cosAngle = max(0.0, dot(lightDir, normal));
    vec3 diffuse = cosAngle * u_LightColor;

    // specular color
    float specularStrength = 0.5;
    vec3 viewDir = normalize(u_CameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * u_LightColor;
    // result
    vec3 result = (diffuse + ambientColor + specular) * u_ObjectColor;

    FragColor = vec4(result, 1.0);
}