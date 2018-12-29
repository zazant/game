#include "game.h"

#include "config.h"
#include "window.h"
#include "state/game_state.h"

#include <GLFW/glfw3.h>

Game::Game(Config &config)
    : mWindow(config), mState(config, mWindow.get()) {

}

void Game::runLoop() {
    while (!mWindow.shouldClose()) {
        float now = glfwGetTime();
        deltaTime = now - lastTime;

        mState.setDeltaTime(deltaTime);

        mState.handleInput();

        mWindow.clear();
        mState.update();
        mState.render();

        mWindow.swapBuffer();
        mWindow.pollEvents();

        lastTime = now;
    }
    glfwTerminate();
}