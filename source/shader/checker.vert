#version 330 core

layout (location = 0) in vec3 Position;

uniform mat4 Projection;
uniform mat4 View;

out vec3 frag_position;

float calculate(vec3 pos) {
  if (pos.x > 0) {
    return 0.0;
  } else {
    return 1.0;
  }
}

void main() {
  frag_position = Position;
  gl_Position = Projection * View * vec4(Position, 1.0);
}