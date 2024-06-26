#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 vColor;
} fs_in;

void main() {
    FragColor = vec4(fs_in.vColor, 1.0);
}