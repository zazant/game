#include "world.h"

#include "config.h"
#include "perlin.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <random>
#include <time.h>

// for intersect
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/intersect.hpp>

namespace {
    glm::vec2 castToVec2(Vertex vertex)
    {
        glm::vec3 temp = *reinterpret_cast<glm::vec3 *>(vertex.positions);
        return {temp.x, temp.z};
    }

    glm::vec2 castToVec2(glm::vec3 vertex)
    {
        return {vertex.x, vertex.z};
    }

    glm::vec3 castToVec3(Vertex vertex)
    {
        return *reinterpret_cast<glm::vec3 *>(vertex.positions);
    }

    // Compute barycentric coordinates (u, v, w) for
    // point p with respect to triangle (a, b, c)
    void barycentric(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c, float &u, float &v, float &w)
    {
        glm::vec2 v0 = b - a, v1 = c - a, v2 = p - a;
        float den = 1.0f / (v0.x * v1.y - v1.x * v0.y);
        v = (v2.x * v1.y - v1.x * v2.y) * den;
        w = (v0.x * v2.y - v2.x * v0.y) * den;
        u = 1.0f - v - w;
    }
}

World::World(Config &config, Player &player)
        : shader("res/shader/checker.vert", "res/shader/checker.frag"),
          mConfig(config),
          mPlayer(player)
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

    unsigned int time_ui = static_cast<unsigned int>( time(NULL) );

    PerlinNoise noise(20);

    int chunkSize = mConfig.INTERNAL_SETTINGS.CHUNK_SIZE;

    float step = 1.0f;

    float corner = -(chunkSize - 1) / 2.0f;

    // create vertices
    float xVertex = corner;
    float zVertex = corner;
    for (int i = 0; i < chunkSize; i++) {
        for (int j = 0; j < chunkSize; j++) {
            Vertex tempVertex = {xVertex, (float) noise.octaveNoise0_1(xVertex / chunkSize, zVertex / chunkSize, 8) * 20, zVertex};
            mMesh.vertices.push_back(tempVertex);

            xVertex += step;
        }
        zVertex += step;
        xVertex = corner;
    }

    // create triangles from vertices
    mMesh.indices.clear();
    for (unsigned int i = 0; i < chunkSize * (chunkSize - 1); i += chunkSize) {
        for (unsigned int j = 0; j < chunkSize - 1; j++) {
            mMesh.indices.push_back(Index{i + j, i + j + chunkSize, i + j + chunkSize + 1});
            mMesh.indices.push_back(Index{i + j, i + j + 1, i + j + chunkSize + 1});
        }
    }

}

void World::update(float deltaTime)
{
    int chunkSize = mConfig.INTERNAL_SETTINGS.CHUNK_SIZE;
    PerlinNoise noise(20);
    float time = glm::cos(glm::sin((float) glfwGetTime()));
    checkCollision();
    // world updating
    for (auto &a : mMesh.vertices)
        a.positions[1] = (float) noise.octaveNoise0_1(a.positions[0] * 10 / chunkSize, a.positions[2] * 10 / chunkSize, time, 1) * 20;
    glBufferData(GL_ARRAY_BUFFER, mMesh.vertices.size() * sizeof(Vertex), mMesh.vertices.data(), GL_STATIC_DRAW);
}

void World::render(const glm::mat4 proj, const glm::mat4 view)
{
    // set shader variables here
    shader.setMat4("Projection", proj);
    shader.setMat4("View", view);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (int) mMesh.indices.size() * 3, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void World::checkCollision()
{
    float playerHeight = mConfig.INTERNAL_SETTINGS.PLAYER_HEIGHT;
    int chunkSize = mConfig.INTERNAL_SETTINGS.CHUNK_SIZE;

    // to avoid bad 0
    glm::vec3 currentPos = mPlayer.getPosition() + 0.000001f;
    // remember: y would be equal to z
    glm::vec2 currentPos2D = castToVec2(mPlayer.getPosition());

    glm::vec2 triangle[3] = {
            {glm::floor(currentPos.x), glm::floor(currentPos.z)},
            {glm::ceil(currentPos.x),  glm::ceil(currentPos.z)},
    };

    glm::vec2 bottomLeft = {glm::ceil(currentPos.x), glm::floor(currentPos.z)};
    glm::vec2 topRight = {glm::floor(currentPos.x), glm::ceil(currentPos.z)};

    if (glm::distance(currentPos2D, bottomLeft) < glm::distance(currentPos2D, topRight)) {
        triangle[2] = bottomLeft;
    } else {
        triangle[2] = topRight;
    }

    glm::vec3 height[3];

    int centerIndex = chunkSize / 2 + chunkSize * (chunkSize / 2);

    // values other than y are for debugging -- remove later
    for (int i = 0; i < 3; i++) {
        int index = centerIndex + (int) triangle[i].x + (int) triangle[i].y * chunkSize;
        height[i] = {mMesh.vertices[index].positions[0], mMesh.vertices[index].positions[1],
                     mMesh.vertices[index].positions[2]};
    }

    float u, v, w;
    barycentric(currentPos2D, triangle[0], triangle[1], triangle[2], u, v, w);
    float tempHeight = u * height[0].y + v * height[1].y + w * height[2].y;

    if (!mConfig.INTERNAL_SETTINGS.FLY || currentPos.y - playerHeight < tempHeight)
        mPlayer.setPosition(glm::vec3(currentPos.x, tempHeight + playerHeight, currentPos.z));
}
