#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in mat4 aInstanceMatrix;

uniform float uOffsetY;
uniform mat4 uViewProj;

out VS_OUT {
    vec2 vTexCoord;
    vec3 vNormal;
    vec3 vFragPos;
} vs_out;

void main() {

    vec3 position = aPos;
    position.y += uOffsetY;
    gl_Position = uViewProj * aInstanceMatrix * vec4(position, 1.0);

    vs_out.vTexCoord = aTexCoord;
    vs_out.vNormal = mat3(transpose(inverse(aInstanceMatrix))) * aNormal;
    vs_out.vFragPos = vec3(aInstanceMatrix * vec4(position, 1.0));
}