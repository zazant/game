#include "world.h"

#include "config.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <random>

World::World(Config &config)
    : shader("res/shader/checker.vert", "res/shader/checker.frag"),
      mConfig(config)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    generateWorld();

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // maybe change to GL_STATIC_DRAW? test out
    glBufferData(GL_ARRAY_BUFFER, mMesh.vertices.size() * sizeof(Vertex), mMesh.vertices.data(), GL_DYNAMIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMesh.indices.size() * sizeof(Index), mMesh.indices.data(), GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    shader.use();
}

void World::generateWorld()
{
    std::default_random_engine engine;
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    int chunkSize = mConfig.INTERNAL_SETTINGS.CHUNK_SIZE;

    float step = 1.0f;

    float corner = -(chunkSize - 1) / 2.0f;

    // create indices
    float xVertex = corner;
    float zVertex = corner;
    for (int i = 0; i < chunkSize; i++) {
        for (int j = 0; j < chunkSize; j++) {
            mMesh.vertices.push_back(Vertex{xVertex, distribution(engine), zVertex});

            zVertex += step;
        }
        xVertex += step;
        zVertex = corner;
    }

    // create heightmap
    mMesh.indices.clear();
    for (unsigned int i = 0; i < chunkSize * (chunkSize - 1); i += chunkSize) {
        for (unsigned int j = 0; j < chunkSize - 1; j++) {
            mMesh.indices.push_back(Index{i + j, i + j + chunkSize, i + j + chunkSize + 1});
            mMesh.indices.push_back(Index{i + j, i + j + 1, i + j + chunkSize + 1});
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

void World::update(float deltaTime) {
    // world updating
//    for (auto &a : mMesh.vertices)
//        a.y += ((rand() % 1000 - 400) / 1000.0f) * deltaTime;
//    glBufferData(GL_ARRAY_BUFFER, mMesh.vertices.size() * sizeof(Vertex), mMesh.vertices.data(), GL_STATIC_DRAW);
}

void World::render(const glm::mat4 proj, const glm::mat4 view)
{
    // set shader variables here
    shader.setMat4("Projection", proj);
    shader.setMat4("View", view);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, mMesh.indices.size() * 3, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
