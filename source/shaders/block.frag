#version 330 core

in vec3 LightDirection;

out vec4 FragColor;

in vec3 frag_position;

void main() {
    vec3 temp_color = vec3(89 / 255.0, 210 / 255.0, 122 / 255.0);
    FragColor = vec4(temp_color, 1.0);
}
