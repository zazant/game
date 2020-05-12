#include "game_state.h"
#include "state.h"

#include "game/world.h"
#include "game/player.h"

#include <GLFW/glfw3.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>

GameState::GameState(json &config, GLFWwindow *w)
        : State(config, w), mPlayer(config, w), mWorld(config, mPlayer)
{}

void GameState::handleInput()
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
        mPlayer.handleMouseClick();

    // close
     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
         glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        mPlayer.setRun(true);
    else {
        mPlayer.setRun(false);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        mPlayer.handleKeyboard(FORWARD);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        mPlayer.handleKeyboard(BACKWARD);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        mPlayer.handleKeyboard(LEFT);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        mPlayer.handleKeyboard(RIGHT);
}

void GameState::update(float deltaTime)
{
    mPlayer.update(deltaTime);
    mWorld.update(deltaTime);
}

void GameState::render(float framerate)
{
    mWorld.render(framerate, mPlayer.getProjectionMatrix(), mPlayer.getViewMatrix(), mPlayer.isMenu());
}

