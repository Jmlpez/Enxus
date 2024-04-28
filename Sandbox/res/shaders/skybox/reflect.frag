#version 330

in vec3 vPosition;
in vec3 vNormal;

out vec4 FragColor;

uniform vec3 uCameraPos;
uniform samplerCube uSkyBoxTexture;

void main() {

    // Incident vector (from view to object)
    vec3 I = normalize(vPosition - uCameraPos);

    // from object to skybox
    vec3 R = reflect(I, normalize(vNormal));

    FragColor = vec4(texture(uSkyBoxTexture, R).rgb, 1.0);
}