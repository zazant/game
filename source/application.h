#pragma once

#include "config.h"
#include "window.h"
#include "state/game_state.h"

class Application {
public:
    explicit Application(Config &config);

    void runLoop();

private:
    Window mWindow;

    // todo: convert to vector
    GameState mState;
};