#version 330 core

out vec4 FragColor;

in vec2 Position;

const int MAX_ITER = 100;

float mandelbrot(vec2 c) {
  vec2 z = vec2(c);
  for (float i = 0.0; i < MAX_ITER; i++) {
    if (abs(dot(z, z)) > 2.0) {
      return (i / MAX_ITER);
    }
    z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
  }
  return 1.0;
}

void main() {
  float result = mandelbrot(Position);
  FragColor = vec4(result, result, result, 1.0);
}