#version 330

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aOffset;

out VS_OUT {
    vec3 vColor;
} vs_out;

uniform mat4 uProj;

void main() {
    gl_Position = uProj * vec4(aPos + aOffset, 0.0, 1.0);
    vs_out.vColor = aColor;
}