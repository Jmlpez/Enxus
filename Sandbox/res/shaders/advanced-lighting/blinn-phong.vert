#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

// declare an interface block; see 'Advanced GLSL' for what these are.
out VS_OUT {
    vec3 vFragPos;
    vec3 vNormal;
    vec2 vTexCoords;
} vs_out;

uniform mat4 uModel;
uniform mat4 uProj;
uniform mat4 uView;

void main() {

    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);

    vs_out.vFragPos = aPos;
    vs_out.vNormal = aNormal;
    vs_out.vTexCoords = aTexCoords;

}