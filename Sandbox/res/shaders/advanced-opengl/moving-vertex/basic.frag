#version 330 core

out vec4 FragColor;

in vec2 vTexCoord;
in vec3 vNormal;

// Example

uniform sampler2D uContainer;
uniform sampler2D uFace;

void main() {

    FragColor = mix(texture(uContainer, vTexCoord), texture(uFace, vTexCoord), 0.2);

}