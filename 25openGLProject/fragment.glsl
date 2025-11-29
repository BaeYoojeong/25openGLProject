#version 330 core

in vec3 vNormal;
uniform vec3 uColor;

out vec4 FragColor;

void main() {
    vec3 N = normalize(vNormal);
    float d = max(N.z, 0.0);
    float intensity = 0.4 + 0.6 * d;

    FragColor = vec4(uColor * intensity, 1.0);
}
