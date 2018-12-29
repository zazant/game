#pragma once

#include "config.h"
#include "window.h"
#include "state/game_state.h"

class Game {
public:
    explicit Game(Config &config);

    void runLoop();

private:
    Window mWindow;

    float lastTime = 0.0f;
    float deltaTime = 0.0f;

    // todo: convert to vector
    GameState mState;
};