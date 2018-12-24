#include "application.h"

#include "config.h"
#include "window.h"
#include "state/state.h"

#include <GLFW/glfw3.h>

Application::Application(const Config &config)
: mWindow(config), mState(config, mWindow.get()) {

}

void Application::runLoop() {
  while (!mWindow.shouldClose()) {
    mState.handleInput();
    mState.update();
    mState.render();

    mWindow.clear();
    mWindow.swapBuffer();
    mWindow.pollEvents();
  }
  glfwTerminate();
}