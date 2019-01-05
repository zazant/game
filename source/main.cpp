#include <iostream>

#include "config.h"
#include "state/game_state.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stack>

static float now = 0.0f;
static float lastTime = 0.0f;
static float deltaTime = 0.0f;

static std::stack<std::unique_ptr<State>> states;
static Config config;

static void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);

    config.WIDTH = width;
    config.HEIGHT = height;
}

int main()
{
    // TODO: add config.ini

    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // fix compilation on OS X
#endif

    // glfw window creation
    GLFWwindow *window = glfwCreateWindow(config.WIDTH, config.HEIGHT, "Game", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(0);

    states.push(std::make_unique<GameState>(config, window));

    glm::vec3 skyColor = {171, 212, 220};
    skyColor /= 255;

    while (!glfwWindowShouldClose(window)) {
        now = (float) glfwGetTime();
        deltaTime = now - lastTime;

        states.top()->handleInput();

        // clear window background and buffer
        glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        states.top()->update(deltaTime);
        states.top()->render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        lastTime = now;
    }
    glfwTerminate();

    return 0;
}