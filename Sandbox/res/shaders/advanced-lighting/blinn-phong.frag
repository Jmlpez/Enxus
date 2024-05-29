#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 vFragPos;
    vec3 vNormal;
    vec2 vTexCoords;
} fs_in;

struct Material {
    sampler2D texture_diffuse1;
};

uniform Material material;

uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform bool uBlinn;

void main() {
    vec3 color = texture(material.texture_diffuse1, fs_in.vTexCoords).rgb;
    // ambient
    vec3 ambient = 0.05 * color;
    // diffuse
    vec3 lightDir = normalize(uLightPos - fs_in.vFragPos);
    vec3 normal = normalize(fs_in.vNormal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(uViewPos - fs_in.vFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    if(uBlinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    } else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
    vec3 specular = vec3(0.3) * spec; // assuming bright white light color
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}