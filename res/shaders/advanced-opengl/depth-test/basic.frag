#version 330 core

out vec4 FragColor;

in vec2 vTexCoord;
in vec3 vNormal;

// Example
struct Material {
    sampler2D texture_diffuse1;
};

uniform Material material;
uniform bool uShowDepthValue;

float near = 0.01;
float far = 50;

float LinearizeDepth(float depth) {

    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {

    //FragColor = vec4(vec3(texture2D(material.texture_diffuse1, vTexCoord)), 1.0);
    //FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    vec3 result = vec3(1.0);
    if(uShowDepthValue) {
        float depth = LinearizeDepth(gl_FragCoord.z) / far;
        result = vec3(depth);
    } else {
        result = vec3(texture2D(material.texture_diffuse1, vTexCoord));
    }
    FragColor = vec4(result, 1.0);

}