#include "game_state.h"
#include "state.h"

#include "../config.h"
#include "../world.h"
#include "../player.h"

#include <GLFW/glfw3.h>

#include <iostream>

GameState::GameState(Config &config, GLFWwindow *w)
    : State(config, w), mWorld(config), mPlayer(config, w)
{}

void GameState::handleInput()
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
        mPlayer.handleMouseClick();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

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

void GameState::render()
{
    mWorld.render(mPlayer.getProjectionMatrix(), mPlayer.getViewMatrix());
}

