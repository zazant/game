#pragma once

#include "config.h"
#include "state/game_state.h"

class Game {
public:
    explicit Game(Config &config);

    void runLoop();

    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

    ~Game();

private:
    GLFWwindow *window;

    // todo: convert to vector
    GameState *mState;
};