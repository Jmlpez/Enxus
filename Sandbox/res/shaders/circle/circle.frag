#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec2 vFragCoord;
} fs_in;

void main() {

    float distance = 1 - length(fs_in.vFragCoord);
    float alpha = smoothstep(0.0, 0.005, distance);

    float thickness = 0.5;
    float fade = 0.005;

    alpha *= smoothstep(thickness + fade, thickness, distance);

    vec3 color = vec3(0.3, 0.6, 0.3);

    FragColor.rgb = color;
    FragColor.a = alpha;

}