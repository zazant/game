#pragma once

#include "config.h"

#include <GLFW/glfw3.h>

class Window {
public:
    explicit Window(Config &config);

    GLFWwindow *get();

    bool shouldClose();

    void clear();

    void clear(float r, float g, float b);

    void swapBuffer();

    void pollEvents();

private:
    GLFWwindow *window;
};