#pragma once

#include "game/world.h"
#include "game/player.h"

#include <GLFW/glfw3.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class State {
public:
    State(json &config, GLFWwindow *window)
            : mConfig(config), window(window)
    {};

    virtual void handleInput() = 0;

    virtual void update(float deltaTime) = 0;

    virtual void render(float framerate) = 0;

    virtual ~State() = default;

protected:
    json &mConfig;
    GLFWwindow *window;

};