#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 proj;
uniform mat4 view;
uniform float time;

out vec3 position;

void main() {
  position = aPos + 0.1;
  gl_Position = proj * view * vec4(aPos, 1.0);
}