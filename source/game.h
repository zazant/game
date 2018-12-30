#pragma once

#include "config.h"
#include "state/game_state.h"

#include <utility>

class Game {
public:
    explicit Game(Config &config);

    void runLoop();

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

private:
    GLFWwindow *window;

    // todo: convert to vector
    std::unique_ptr<State> mState;

    float now = 0.0f;
    float lastTime = 0.0f;
    float deltaTime = 0.0f;
};