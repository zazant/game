#pragma once

#include "config.h"
#include "entity.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Player : public Entity {
public:
    Player(Config &config, GLFWwindow *window);

    void update(float dt);

    glm::mat4 getViewMatrix();

    glm::mat4 getProjectionMatrix();

    void setMouse(double x, double y);

    void handleMouseClick();

    void handleKeyboard(Direction direction);

    Config &getConfig();

private:
    Config mConfig;
    GLFWwindow *window;

    float deltaTime = 0.0f;

    float lastX, lastY;
    float yaw, pitch;
    bool firstMouse;
};
