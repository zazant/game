#include "player.h"

#include "config.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace {
  void mouseCallback(GLFWwindow *window, double xpos, double ypos) {

  }
}

Player::Player(const Config &config) 
: mConfig(config) {
  // todo: set init values in better way (or something)
  position = glm::vec3(0.0, 0.0, 0.0);
  target = glm::vec3(0.0, 0.0, -1.0);
  lastX = config.WIDTH / 2.0f;
  lastY = config.HEIGHT / 2.0f;
}

glm::mat4 Player::getViewMatrix() {
  return glm::lookAt(position, position + target, glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 Player::getProjectionMatrix() {
  return glm::perspective(glm::radians(mConfig.FOV), (float)mConfig.WIDTH / (float)mConfig.HEIGHT, 0.1f, 100.0f);
}

void Player::setCallback(GLFWwindow *window) {
  glfwSetCursorPosCallback(window, mouseCallback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Player::handleKeyboard(Direction direction) {

}