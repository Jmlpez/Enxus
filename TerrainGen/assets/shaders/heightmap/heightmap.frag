#version 330 core

out vec4 FragColor;
in vec2 vTexCoord;
in float vHeight;

uniform sampler2D uGrass;

void main() {

    // white color
    //float h = vHeight / 5;	// shift and scale the height into a grayscale value
    //FragColor = vec4(h, h, h, 1.0);
    FragColor = texture2D(uGrass, vTexCoord);

}
