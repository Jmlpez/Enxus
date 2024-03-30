#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D u_TextureWood;
uniform sampler2D u_TextureFace;

void main() {
    // FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    //FragColor = vertexColor;
    //FragColor = uColor;
    //FragColor = ourVertexColor;
    FragColor = mix(texture(u_TextureWood, TexCoord), texture(u_TextureFace, TexCoord), 0.2);

}