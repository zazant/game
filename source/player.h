#pragma once

#include "config.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Player {
public:
    Player(Config &config, GLFWwindow *window);

    void update();

    glm::mat4 getViewMatrix();

    glm::mat4 getProjectionMatrix();

    void setMouse(double x, double y);

    void handleMouseClick();

    void handleKeyboard(Direction direction);

private:
    Config mConfig;
    GLFWwindow *window;

    float lastX, lastY;
    float yaw, pitch;
    bool firstMouse;
    Direction tempPos;

    glm::vec3 position;
    glm::vec3 target;
};
