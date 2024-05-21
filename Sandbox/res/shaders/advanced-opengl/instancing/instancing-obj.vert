#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
// (location 2) --> normals omitted
layout(location = 3) in mat4 aInstanceMatrix;

uniform mat4 uView;
uniform mat4 uProj;

out VS_OUT {
    vec2 vTexCoord;
} vs_out;

void main() {

    gl_Position = uProj * uView * aInstanceMatrix * vec4(aPos, 1.0);
    vs_out.vTexCoord = aTexCoord;

}