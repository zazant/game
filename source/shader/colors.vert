#version 330 core

layout (location = 0) in vec3 Position;

uniform mat4 Projection;
uniform mat4 View;

out vec3 frag_position;

void main() {
  frag_position = vec3(Position.x, Position.y, Position.z);
  gl_Position = Projection * View * vec4(Position, 1.0);
}