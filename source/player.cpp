#include "player.h"

#include "config.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace {
    void cursorPosCallback(GLFWwindow *window, double x, double y)
    {
        Player *player = (Player *) glfwGetWindowUserPointer(window);
        player->setMouse(x, y);
    }
};

Player::Player(Config &config, GLFWwindow *w)
    : Entity(glm::vec3(0.0, 1.0, 0.0)),
      mConfig(config),
      window(w),
      yaw(-90.0f),
      pitch(0.0f),
      firstMouse(true)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    // disable cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    rotation = glm::normalize(front);
}

void Player::update(float dt)
{
    deltaTime = dt;
}

glm::mat4 Player::getViewMatrix()
{
    return glm::lookAt(position, position + rotation, glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 Player::getProjectionMatrix()
{
    return glm::perspective(glm::radians(mConfig.FOV), (float) mConfig.WIDTH / (float) mConfig.HEIGHT, 0.01f, 100.0f);
}

void Player::setMouse(double x, double y)
{
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }
    float xOffset = x - lastX;
    float yOffset = lastY - y;
    lastX = x;
    lastY = y;
    yaw += xOffset * mConfig.SENSITIVITY;
    pitch += yOffset * mConfig.SENSITIVITY;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    rotation = glm::normalize(front);
}

void Player::handleMouseClick()
{

}

void Player::handleKeyboard(Direction direction)
{
    float adjustedSpeed = mConfig.INTERNAL_SETTINGS.MOVEMENT_SPEED * deltaTime;

    switch (direction) {
        case FORWARD:
            position += rotation * adjustedSpeed;
            break;
        case BACKWARD:
            position -= rotation * adjustedSpeed;
            break;
        case LEFT:
            position -= glm::normalize(glm::cross(rotation, glm::vec3(0.0, 1.0, 0.0))) * adjustedSpeed;
            break;
        case RIGHT:
            position += glm::normalize(glm::cross(rotation, glm::vec3(0.0, 1.0, 0.0))) * adjustedSpeed;
            break;
    }
}
