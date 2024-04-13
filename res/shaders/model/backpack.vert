#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vFragPos;

void main() {

    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
    vNormal = aNormal;
    vFragPos = vec3(uModel * vec4(aPos, 1.0));

    vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    vTexCoord = aTexCoord;

}