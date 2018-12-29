#pragma once

#include "state.h"

#include "../config.h"
#include "../world.h"
#include "../player.h"

#include <GLFW/glfw3.h>

// todo: make multiple states

class GameState : public State {
public:
    GameState(Config &config, GLFWwindow *window);

    void handleInput() override;

    void update() override;

    void render() override;

private:
    GLFWwindow *window;

    float deltaTime = 0.0f;

    World mWorld;
    Player mPlayer;
};