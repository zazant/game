#include "player.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <iostream>

namespace {
    void cursorPosCallback(GLFWwindow *window, double x, double y)
    {
        Player *player = (Player *) glfwGetWindowUserPointer(window);
        player->setMouse(x, y);
    }

//    void escapeCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    void escapeCallback(GLFWwindow* window, int button, int action, int mods)
    {
        Player *player = (Player *) glfwGetWindowUserPointer(window);
//        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            if (!player->isMenu()) {
                player->setMenu(!player->isMenu());

                double cursorX;
                double cursorY;
                glfwGetCursorPos(window, &cursorX, &cursorY);

                player->setPausedCursorLocation(glm::vec2(cursorX, cursorY));

                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                glfwSetCursorPosCallback(window, NULL);
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                player->setMenu(!player->isMenu());

                glm::vec2 cursorPos = player->getPausedCursorLocation();
                glfwSetCursorPos(window, cursorPos.x, cursorPos.y);

                glfwSetCursorPosCallback(window, cursorPosCallback);
            }
        }
    }
};

Player::Player(json &config, GLFWwindow *w)
        : Entity(glm::vec3(0.0, (float) config["internal_settings"]["player_height"] + 1.0, 0.0), glm::vec3(0.0),
                 glm::vec3(0.0, (float) config["internal_settings"]["player_height"], 0.0)),
          mConfig(config),
          window(w),
          yaw(0.0f),
          pitch(0.0f),
          firstMouse(true),
          run(false),
          menu(false)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    // disable cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetMouseButtonCallback(w, escapeCallback);
//    glfwSetKeyCallback(w, escapeCallback);

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
    return glm::perspective(glm::radians((float) mConfig["fov"]), (float) mConfig["width"] / (float) mConfig["height"], 0.01f, 100.0f);
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
    yaw += xOffset * (float) mConfig["sensitivity"];
    pitch += yOffset * (float) mConfig["sensitivity"];

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
    float adjustedSpeed = (float) mConfig["internal_settings"]["movement_speed"] * deltaTime * (run ? (float) mConfig["internal_settings"]["run_mul"] : 1.0f);

    if (mConfig["internal_settings"]["fly"])
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
    else
        switch (direction) {
            case FORWARD:
                position += glm::normalize(glm::vec3(rotation.x, 0.0, rotation.z)) * adjustedSpeed;
                break;
            case BACKWARD:
                position -= glm::normalize(glm::vec3(rotation.x, 0.0, rotation.z)) * adjustedSpeed;
                break;
            case LEFT:
                position -= glm::normalize(glm::cross(rotation, glm::vec3(0.0, 1.0, 0.0))) * adjustedSpeed;
                break;
            case RIGHT:
                position += glm::normalize(glm::cross(rotation, glm::vec3(0.0, 1.0, 0.0))) * adjustedSpeed;
                break;
        }
}

json *Player::getConfig()
{
    return &mConfig;
}

bool Player::isMenu() const {
    return menu;
}

void Player::setMenu(bool menu) {
    Player::menu = menu;
}

const glm::vec2 &Player::getPausedCursorLocation() const {
    return pausedCursorLocation;
}

void Player::setPausedCursorLocation(const glm::vec2 &pausedCursorLocation) {
    Player::pausedCursorLocation = pausedCursorLocation;
}

GLFWwindow *Player::getWindow() const {
    return window;
}

void Player::setRun(bool run) {
    Player::run = run;
}
