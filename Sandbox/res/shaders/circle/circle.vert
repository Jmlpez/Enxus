#version 330

layout(location = 0) in vec2 aPos;

out VS_OUT {
    vec2 vFragCoord;
} vs_out;

uniform mat4 uProj;

void main() {

    gl_Position = uProj * vec4(aPos.xy, 0.0, 1.0);
    vs_out.vFragCoord = aPos.xy;
}