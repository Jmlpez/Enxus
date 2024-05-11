#version 330 core

out vec4 FragColor;
in vec2 vTexCoord;
in float vHeight;

uniform sampler2D uGrass;
uniform sampler2D uSnow;

void main() {

    // white color
    //float h = vHeight / 5;	// shift and scale the height into a grayscale value
    //FragColor = vec4(h, h, h, 1.0);
    vec4 grassTex = texture2D(uGrass, vTexCoord);
    vec4 snowTex = texture2D(uSnow, vTexCoord);
    if(vHeight < 1.0)
        FragColor = grassTex;
    else {
        float alpha = (vHeight - 1);

        FragColor = mix(grassTex, snowTex, alpha);
    }

}
