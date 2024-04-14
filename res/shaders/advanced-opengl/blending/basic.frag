#version 330 core

out vec4 FragColor;

in vec2 vTexCoord;
in vec3 vNormal;

// Example
struct Material {
    sampler2D texture_diffuse1;
};

uniform Material material;
uniform bool uShowDepthValue;

void main() {

    vec4 texColor = texture2D(material.texture_diffuse1, vTexCoord);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;

}