#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

//uniform mat4 uModel; // the terrain does not need to be moved so this matrix is not necessary
uniform mat4 uViewProj;

out VS_OUT {
    vec3 vVertexPos;
    vec2 vTexCoord;
    vec3 vNormal;
    vec3 vFragPos;
} vs_out;

void main() {

    gl_Position = uViewProj * vec4(aPos, 1.0);
    vs_out.vVertexPos = aPos;
    vs_out.vTexCoord = aTexCoord;

    // vs_out.vFragPos = vec3(uModel * vec4(aPos, 1));
    // vs_out.vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    vs_out.vFragPos = aPos;
    vs_out.vNormal = aNormal;
}