#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out VS_OUT {
    out float vHeight;
    out vec2 vTexCoord;
} vs_out;

void main() {

    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
    vs_out.vHeight = aPos.y;
    vs_out.vTexCoord = aTexCoord;
}