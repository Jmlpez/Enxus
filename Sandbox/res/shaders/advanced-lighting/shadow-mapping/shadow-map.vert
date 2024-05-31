#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uLightSpaceMatrix;

out VS_OUT {
    vec2 vTexCoord;
    vec3 vNormal;
    vec3 vFragPos;
    vec4 vFragPosLightSpace;
} vs_out;

void main() {

    vs_out.vFragPos = vec3(uModel * vec4(aPos, 1.0));
    gl_Position = uProj * uView * vec4(vs_out.vFragPos, 1.0);

    vs_out.vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    vs_out.vTexCoord = aTexCoord;

    vs_out.vFragPosLightSpace = uLightSpaceMatrix * vec4(aPos, 1.0);

}