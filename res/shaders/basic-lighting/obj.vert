#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 Normal;
out vec3 FragPos;

void main() {
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
    FragPos = vec3(uModel * vec4(aPos, 1.0));

    Normal = mat3(transpose(inverse(uModel))) * aNormal;
}