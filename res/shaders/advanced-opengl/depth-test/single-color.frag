#version 330
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
};

uniform Material material;
vec2 vTexCoord;

void main() {
    vec3 result = vec3(texture2D(material.texture_diffuse1, vTexCoord));
    result *= 0;
    result = vec3(0.04, 0.28, 0.26);
    FragColor = vec4(result, 1.0);
}