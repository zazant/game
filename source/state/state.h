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

    virtual void update() = 0;

    virtual void render() = 0;

    Config &getConfig() const
    {
        return *mConfig;
    }

protected:
    Config *mConfig;
    GLFWwindow *window;

    void startTick()
    {
        now = static_cast<float>(glfwGetTime());
        deltaTime = now - lastTime;
    }

    void endTick()
    {
        lastTime = now;
    }

    float getDeltaTime() const
    {
        return deltaTime;
    }

    float now = 0.0f;
    float lastTime = 0.0f;
    float deltaTime = 0.0f;
};