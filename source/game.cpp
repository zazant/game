#include "game.h"

#include "config.h"
#include "state/game_state.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>

Game::Game(Config &config)
    : window(nullptr),
      mState(nullptr)
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    window = glfwCreateWindow(config.WIDTH, config.HEIGHT, "Game", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, Game::framebufferSizeCallback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(0);

    // test

    mState = std::make_unique<GameState>(config, window);
}

void Game::runLoop()
{
    while (!glfwWindowShouldClose(window)) {
        now = (float) glfwGetTime();
        deltaTime = now - lastTime;

        mState->handleInput();

        // clear window background and buffer
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mState->update(deltaTime);
        mState->render();

        glfwSwapBuffers(window);
        glfwPollEvents();

        lastTime = now;
    }
    glfwTerminate();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Game::framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);

    // get config from player object
    auto *player = (Player *) glfwGetWindowUserPointer(window);
    Config &config = player->getConfig();
    config.WIDTH = width;
    config.HEIGHT = height;
}
