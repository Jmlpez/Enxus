#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out float vHeight;
out vec2 vTexCoord;

void main() {

    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);
    vHeight = aPos.y;
    vTexCoord = aTexCoord;
}