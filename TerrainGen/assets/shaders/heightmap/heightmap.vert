#version 330

layout(location = 0) in vec3 aPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out float vHeight;

void main() {

    vHeight = aPos.y;
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
}