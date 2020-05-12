#pragma once

#include "state.h"

#include "game/world.h"
#include "game/player.h"

#include <GLFW/glfw3.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

// todo: make multiple states

class GameState : public State {
public:
    GameState(json &config, GLFWwindow *window);

    void handleInput() override;

    void update(float deltaTime) override;

    void render(float framerate) override;

private:
    World mWorld;
    Player mPlayer;
};