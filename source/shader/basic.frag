#version 330 core

out vec4 FragColor;

in vec2 Position;

void main() {
  FragColor = vec4(Position, 1.0, 1.0);
}