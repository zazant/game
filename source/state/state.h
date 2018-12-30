#pragma once

#include "../config.h"
#include "../world.h"
#include "../player.h"

#include <GLFW/glfw3.h>

class State {
public:
    State(Config &config, GLFWwindow *window)
            : mConfig(&config), window(window)
    {};

    virtual void handleInput() = 0;

    virtual void update(float deltaTime) = 0;

    virtual void render() = 0;

    Config &getConfig() const
    {
        return *mConfig;
    }

    virtual ~State() = default;

protected:
    Config *mConfig;
    GLFWwindow *window;

};