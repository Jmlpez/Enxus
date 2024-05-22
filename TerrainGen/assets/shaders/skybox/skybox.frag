#version 330

out vec4 FragColor;

in vec3 vTexCoords;

uniform samplerCube uSkyBoxTexture;

void main() {
    FragColor = texture(uSkyBoxTexture, vTexCoords);
}