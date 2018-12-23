#include "application.h"

Application::Application(const Config &config)
: mWindow(config) {

}

void Application::runLoop() {
  while (!mWindow.shouldClose()) {
    mWindow.clear();
    mWindow.swapBuffer();
    mWindow.pollEvents();
  }
  glfwTerminate();
}