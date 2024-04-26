#version 330

layout(location = 0) in vec3 aPos;

out vec3 vTexCoords;

// dont need model (always ll'be at the center)
uniform mat4 uProj;
uniform mat4 uView;

void main() {

    vec4 pos = uProj * uView * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
    vTexCoords = aPos;
}