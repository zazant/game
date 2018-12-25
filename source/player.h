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
    Player(const Config &config);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void handleMouse(GLFWwindow *window);
    void stopPressing();
    void handleKeyboard(Direction direction);

  private:
    glm::vec3 position;
    glm::vec3 target;

    Config mConfig;
};
