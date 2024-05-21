#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec2 vTexCoord;
} fs_in;

// Example
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform Material material;

void main() {

    FragColor = texture2D(material.texture_diffuse1, fs_in.vTexCoord);

}