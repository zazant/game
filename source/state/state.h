#pragma once

#include "../config.h"
#include "../world.h"
#include "../player.h"

#include <GLFW/glfw3.h>

class State {
  public:
    State(Config &config) : mConfig(config) {};
    virtual void handleInput() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
  
  private:
    Config &mConfig;
};