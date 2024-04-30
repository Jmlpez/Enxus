#version 330 core

out vec4 FragColor;
in float vHeight;

void main() {

    // white color
    float h = (vHeight + (0.016 * 2)) / (1.0 * 2);	// shift and scale the height into a grayscale value
    FragColor = vec4(h, h, h, 1.0);

}
