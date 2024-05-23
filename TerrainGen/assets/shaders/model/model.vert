#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out VS_OUT {
    vec2 vTexCoord;
    vec3 vNormal;
    vec3 vFragPos;
} vs_out;

void main() {

    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);

    vs_out.vTexCoord = aTexCoord;
    vs_out.vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    vs_out.vFragPos = vec3(uModel * vec4(aPos, 1.0));
}