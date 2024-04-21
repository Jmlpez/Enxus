#version 330 core

out vec4 FragColor;

in vec2 vTexCoord;
in vec3 vNormal;

// Example
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform Material material;

void main() {
    // FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    //FragColor = vertexColor;
    //FragColor = uColor;
    //FragColor = ourVertexColor;
    FragColor = texture2D(material.texture_diffuse1, vTexCoord);

}