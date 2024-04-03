#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 2) in vec3 aNormal;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;

out vec3 Normal;
out vec3 FragPos;

void main() {
    gl_Position = u_Proj * u_View * u_Model * vec4(aPos, 1.0);
    FragPos = vec3(u_Model * vec4(aPos, 1.0));

    Normal = mat3(transpose(inverse(u_Model))) * aNormal;
}