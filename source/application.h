#pragma once

#include "config.h"
#include "window.h"

class Application {
  public:
    Application(const Config &config);
    void runLoop();
  private:
    Window mWindow;
};