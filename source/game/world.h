#pragma once

#include "player.h"
#include "config.h"
#include "shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

struct Vertex {
    GLfloat positions[3];
    GLfloat normal[3];
};

struct Index {
    GLuint positions[3];
};

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<Index> indices;
};

struct Chunk {
    glm::vec2 offset;
    Mesh mesh;
};

class World {
public:
    explicit World(Config &config, Player &player);

    void generateWorld();

    void update(float deltaTime);

    void render(float framerate, glm::mat4 proj, glm::mat4 view, bool menu);

private:
    void checkCollision();

    float getMapHeight(int x, int y);

    GLuint VAO, VBO, EBO;
    Shader shader;
    Config &mConfig;
    Player &mPlayer;
    std::vector<float> mapData;
    int mapSize;

    // temporary
    Mesh mMesh;
};
