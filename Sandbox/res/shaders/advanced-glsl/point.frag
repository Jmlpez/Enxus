#version 330 core

out vec4 FragColor;

in VS_OUT {
    float vAlpha;
    vec2 vTexCoord;
} fs_in;

// Example

uniform sampler2D uContainer;
uniform sampler2D uFace;

void main() {

    FragColor = mix(texture(uContainer, fs_in.vTexCoord), texture(uFace, fs_in.vTexCoord), fs_in.vAlpha);

    // if(gl_FragCoord.x < 600) {
    //     FragColor = texture(uContainer, vTexCoord);
    // } else {
    //     FragColor = mix(texture(uContainer, vTexCoord), texture(uFace, vTexCoord), 0.2);
    // }

}