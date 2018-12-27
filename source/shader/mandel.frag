#version 330 core

out vec4 FragColor;

in vec3 frag_position;

const int MAX_ITER = 20;

float mandelbrot(vec2 c) {
  vec2 z = vec2(c);
  for (float i = 0.0; i < MAX_ITER; i++) {
    if (abs(dot(z, z)) > 4.0) {
      return (i / MAX_ITER);
    }
    z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
  }
  return 1.0;
}

void main() {
  float result = mandelbrot(vec2(frag_position.x, frag_position.z) * 10.0) + 0.1;
  FragColor = vec4(result, result, result, 1.0);
}