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
        mState.handleInput();

        mWindow.clear();
        mState.update();
        mState.render();

        mWindow.swapBuffer();
        mWindow.pollEvents();
    }
    glfwTerminate();
}