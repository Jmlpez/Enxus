#version 330

layout(location = 0) in vec3 aPos;
// location 2 is texture coordinates
layout(location = 2) in vec3 aNormal;

out vec3 vPosition;
out vec3 vNormal;

uniform mat4 uModel;
uniform mat4 uProj;
uniform mat4 uView;

void main() {

    vNormal = mat3(transpose(inverse(uModel))) * aNormal; // Normals in world space
    vPosition = vec3(uModel * vec4(aPos, 1.0)); // vertices in world space
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);

}