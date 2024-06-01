#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 3) in mat4 aInstanceMatrix;

uniform mat4 uLightSpaceMatrix;
uniform bool uUseInstancing;
uniform float uOffsetY;

/*
I use this approach to avoid having two shaders
The shader binding/unbinding process is costly
so I think this works well
*/

void main() {
    vec4 finalPosition = vec4(0.0);
    if(uUseInstancing) {
        vec3 transformPos = aPos;
        transformPos.y += uOffsetY;
        finalPosition = uLightSpaceMatrix * aInstanceMatrix * vec4(transformPos, 1.0);
    } else {
        // The terrain does not need a model matrix
        finalPosition = uLightSpaceMatrix * vec4(aPos, 1.0);
    }
    gl_Position = finalPosition;
}