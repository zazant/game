#include "player.h"

#include "config.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>


Player::Player(const Config &config) 
: mConfig(config),
  position(glm::vec3(0.0, 0.0, 0.0)),
  target(glm::vec3(0.0, 0.0, -1.0)) {
  // todo: set init values in better way (or something)
  
}

glm::mat4 Player::getViewMatrix() {
  return glm::lookAt(position, position + target, glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 Player::getProjectionMatrix() {
  return glm::perspective(glm::radians(mConfig.FOV), (float)mConfig.WIDTH / (float)mConfig.HEIGHT, 0.1f, 100.0f);
}

// add mouse callback? or implement, at least
void Player::handleMouse(GLFWwindow *window) {

}

void Player::handleKeyboard(Direction direction) {
  // if direction == ...
}
