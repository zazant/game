#version 330 core

out vec4 FragColor;

in vec3 frag_position;

void main() {
    FragColor = vec4(sin(frag_position) * 0.5 + 0.5, 1.0);
}
