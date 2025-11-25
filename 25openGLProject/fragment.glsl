#version 330 core
in vec3 vNormal;
out vec4 FragColor;

void main() {
    vec3 N = normalize(vNormal);
    float d = max(N.z,0.0);
    FragColor = vec4(0.4 + d*0.6, 0.3, 0.3, 1.0);
}
