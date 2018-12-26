#include "player.h"

#include "config.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace {
  void cursorPosCallback(GLFWwindow *window, double x, double y) {
    Player *player = (Player *) glfwGetWindowUserPointer(window);
    (*player).setMouse(x, y);
  }
};

Player::Player(Config &config, GLFWwindow *w) 
: mConfig(config),
  window(w),
  position(glm::vec3(0.0, 0.0, 3.0)),
  yaw(-90.0f),
  firstMouse(true) {
  // todo: set init values in better way (or something)
  glfwSetWindowUserPointer(window, this);
  glfwSetCursorPosCallback(window, cursorPosCallback);
  // disable cursor
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glm::vec3 front;
  front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
  front.y = sin(glm::radians(pitch));
  front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
  target = glm::normalize(front);
}

void Player::update() {
}

glm::mat4 Player::getViewMatrix() {
  return glm::lookAt(position, position + target, glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 Player::getProjectionMatrix() {
  return glm::perspective(glm::radians(mConfig.FOV), (float)mConfig.WIDTH / (float)mConfig.HEIGHT, 0.1f, 100.0f);
}

void Player::setMouse(double x, double y) {
  if (firstMouse) {
    lastX = x;
    lastY = y;
    firstMouse = false;
  }
  float xOffset = x - lastX;
  float yOffset = lastY - y;
  lastX = x;
  lastY = y;
  yaw += xOffset * mConfig.SENSITIVITY;
  pitch += yOffset * mConfig.SENSITIVITY;

  if(pitch > 89.0f)
    pitch =  89.0f;
  if(pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
  front.y = sin(glm::radians(pitch));
  front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
  target = glm::normalize(front);
}

// add mouse callback? or implement, at least
void Player::handleMouseClick() {

}

void Player::handleKeyboard(Direction direction) {
  const float cameraSpeed = 0.05f;
  switch (direction) {
    case FORWARD:
      position += target * cameraSpeed;
      break;
    case BACKWARD:
      position -= target * cameraSpeed;
      break;
    case LEFT:
      position -= glm::cross(target, glm::vec3(0.0, 1.0, 0.0)) * cameraSpeed;
      break;
    case RIGHT:
      position += glm::cross(target, glm::vec3(0.0, 1.0, 0.0)) * cameraSpeed;
      break;
  }
}
