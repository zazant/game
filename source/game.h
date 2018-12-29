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

    // todo: convert to vector
    GameState mState;
};