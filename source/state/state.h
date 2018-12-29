#pragma once

#include "../config.h"
#include "../world.h"
#include "../player.h"

#include <GLFW/glfw3.h>

class State {
public:
    State(Config &config) : mConfig(config) {};

    virtual void handleInput() = 0;

    virtual void update() = 0;

    virtual void render() = 0;

    float getDeltaTime() const {
        return deltaTime;
    }

    Config &getConfig() const {
        return mConfig;
    }

    void setDeltaTime(float deltaTime) {
        State::deltaTime = deltaTime;
    }

private:
    Config &mConfig;

    float deltaTime = 0.0f;
};