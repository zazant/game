#pragma once

#include "player.h"
#include "config.h"
#include "shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

struct Vertex {
    GLfloat x, y, z;
};

struct Index {
    GLuint x, y, z;
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
};

class World {
public:
    explicit World(Config &config);

    void generateWorld();

    void update(float deltaTime);

    void render(glm::mat4 proj, glm::mat4 view);

private:
    GLuint VAO, VBO, EBO;
    Shader shader;
    Config mConfig;

    // temporary
    Mesh mMesh;
};
