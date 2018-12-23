#pragma once

#include "config.h"

#include <GLFW/glfw3.h>

class Application {
public:
  Application(const Config &config);

  // main loop
  void runLoop();

private:
  GLFWwindow *mWindow;

  Config mConfig;
};
