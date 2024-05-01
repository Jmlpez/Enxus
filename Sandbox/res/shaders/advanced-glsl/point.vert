#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
//layout(location = 2) in vec3 aNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out VS_OUT {
    float vAlpha;
    vec2 vTexCoord;
} vs_out;

void main() {

    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
    gl_PointSize = gl_Position.z;
    vs_out.vTexCoord = aTexCoord;
    vs_out.vAlpha = 0.5;
    //vNormal = aNormal;
}