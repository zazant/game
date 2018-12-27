#version 330 core

out vec4 FragColor;

in vec3 frag_position;

void main() {
    FragColor = vec4(vec3(abs(floor(mod(frag_position.x * 8, 2)) - floor(mod(frag_position.z * 8, 2)))), 1.0);
}
