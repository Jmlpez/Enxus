#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 vNormal;
out vec2 vTexCoord;

void main() {

    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
    vTexCoord = aTexCoord;
    vNormal = aNormal;
}