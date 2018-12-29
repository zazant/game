#pragma once

#include "player.h"
#include "config.h"
#include "shader.h"

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

struct Mesh {
  std::vector<GLfloat> vertices;
  std::vector<GLuint> indices;
};

class World {
  public:
    World(Config &config);
    void generateWorld();
    void render(const glm::mat4 proj, const glm::mat4 view);

  private:
    GLuint VAO, VBO, EBO;
    Shader shader;
    Config mConfig;

    // temporary
    Mesh mMesh;
};
