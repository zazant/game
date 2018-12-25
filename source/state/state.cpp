#include "state.h"

#include "../config.h"
#include "../world.h"
#include "../player.h"

#include <GLFW/glfw3.h>

State::State(const Config &config, GLFWwindow *w)
: mWorld(config), mPlayer(config), window(w) {
  mPlayer.setCallback(window);
}

void State::handleInput() {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    mPlayer.handleKeyboard(FORWARD);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    mPlayer.handleKeyboard(BACKWARD);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    mPlayer.handleKeyboard(LEFT);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    mPlayer.handleKeyboard(RIGHT);
}

void State::update() {

}

void State::render() {
  mWorld.render(mPlayer.getProjectionMatrix(), mPlayer.getViewMatrix());
}