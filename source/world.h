#pragma once

#include "player.h"
#include "config.h"
#include "shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

struct Vertex {
    GLfloat positions[3];
};

struct Index {
    GLuint positions[3];
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
};

class World {
public:
    explicit World(Config &config, Player &player);

    void generateWorld();

    void update(float deltaTime);

    void render(glm::mat4 proj, glm::mat4 view);

private:
    void checkCollision();

    GLuint VAO, VBO, EBO;
    Shader shader;
    Config &mConfig;
    Player &mPlayer;

    // temporary
    Mesh mMesh;
};
