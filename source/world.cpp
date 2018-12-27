#include "world.h"

#include "config.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <random>

World::World(Config &config) 
: shader("../source/shader/checker.vert", "../source/shader/checker.frag"),
  mConfig(config) {
  mMesh.vertices = std::vector<GLfloat>();

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  generateWorld();

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, mMesh.vertices.size() * sizeof(GLfloat), mMesh.vertices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh.indices.size() * sizeof(GLuint), mMesh.indices.data(), GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

  shader.use();
}

void World::generateWorld() {
  std::default_random_engine engine;
  std::uniform_real_distribution<float> distribution(-0.0f, 0.1f);

  const int CHUNK_SIZE = 3;

  float step = 2.0 / (CHUNK_SIZE - 1);

  float xVertex = -1.0;
  float zVertex = -1.0;
  for (int i = 0; i < CHUNK_SIZE; i++) {
    for (int j = 0; j < CHUNK_SIZE; j++) {
      mMesh.vertices.push_back(xVertex);
      mMesh.vertices.push_back(distribution(engine));
      mMesh.vertices.push_back(zVertex);

      zVertex += step;
    }
    xVertex += step;
    zVertex = -1.0;
  }

  for (int i = 0; i < CHUNK_SIZE * (CHUNK_SIZE - 1); i += CHUNK_SIZE) {
    for (int j = 0; j < CHUNK_SIZE - 1; j++) {
      mMesh.indices.push_back(i + j);
      mMesh.indices.push_back(i + j + CHUNK_SIZE);
      mMesh.indices.push_back(i + j + CHUNK_SIZE + 1);

      mMesh.indices.push_back(i + j);
      mMesh.indices.push_back(i + j + 1);
      mMesh.indices.push_back(i + j + CHUNK_SIZE + 1);
    }
  }

  // // debug
  // int remainder = 0;
  // for (auto &a : mMesh.vertices) {
  //   std::cout << a << " ";
  //   remainder++;
  //   if (remainder % 3 == 0) {
  //     std::cout << "\n";
  //   }
  // }

  // std::cout << "---------------------------" << std::endl;

  // // debug
  // remainder = 0;
  // for (auto &a : mMesh.indices) {
  //   std::cout << a << " ";
  //   remainder++;
  //   if (remainder % 3 == 0) {
  //     std::cout << "\n";
  //   }
  // }

}

void World::render(const glm::mat4 proj, const glm::mat4 view) {
  // set shader variables here
  shader.setMat4("Projection", proj);
  shader.setMat4("View", view);

  glBindVertexArray(VAO);
  // glDrawArrays(GL_TRIANGLES, 0, 21 * 21);
  glDrawElements(GL_TRIANGLES, mMesh.indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
