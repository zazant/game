#version 330 core

out vec4 FragColor;

in vec3 frag_position;

void main() {
  FragColor = vec4(frag_position, 1.0);
}
