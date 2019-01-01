#include "world.h"

#include "config.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <random>

// for intersect
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>

namespace {
    glm::vec3 castToVec3(Vertex vertex) {
        return *reinterpret_cast<glm::vec3*>(vertex.positions);
    }

    glm::vec3 castToVec3(Index vertex) {
        return *reinterpret_cast<glm::vec3*>(vertex.positions);
    }

    // Compute barycentric coordinates (u, v, w) for
    // point p with respect to triangle (a, b, c)
    void calculateBarycentric(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c, float &u, float &v, float &w)
    {
        glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
        float d00 = glm::dot(v0, v0);
        float d01 = glm::dot(v0, v1);
        float d11 = glm::dot(v1, v1);
        float d20 = glm::dot(v2, v0);
        float d21 = glm::dot(v2, v1);
        float denom = d00 * d11 - d01 * d01;
        v = (d11 * d20 - d01 * d21) / denom;
        w = (d00 * d21 - d01 * d20) / denom;
        u = 1.0f - v - w;
    }

    glm::vec3 intersectTriangle(glm::vec3 player, glm::vec3 direction, glm::vec3 corner1, glm::vec3 corner2, glm::vec3 corner3)
    {
        float weight1;
        float weight2;
        float weight3;

        calculateBarycentric(player, corner1, corner2, corner3, weight1, weight2, weight3);

        return glm::vec3(corner1 * weight1);
    }

    glm::vec3 collideQuad(glm::vec3 player, glm::vec3 topRight, glm::vec3 topLeft, glm::vec3 bottomLeft, glm::vec3 bottomRight)
    {
        if (glm::distance(topLeft, player) < glm::distance(bottomRight, player)) {
            glm::vec3 position;
            position = intersectTriangle(player, glm::vec3(0.0, -1.0, 0.0), bottomLeft, topLeft, topRight);
            std::cout << "triangle: " << bottomLeft.y << " " << topLeft.y << " " << topRight.y << std::endl;
            std::cout << "result: " << position.y << std::endl;
            return position;
        } else {
            glm::vec3 position;
            position = intersectTriangle(player, glm::vec3(0.0, -1.0, 0.0), bottomLeft, topRight, bottomRight);
            std::cout << "triangle: " << bottomLeft.y << " " << topLeft.y << " " << topRight.y << std::endl;
            std::cout << "result: " << position.y << std::endl;
            return position;
        }
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

    int chunkSize = mConfig.INTERNAL_SETTINGS.CHUNK_SIZE;

    float step = 1.0f;

    float corner = -(chunkSize - 1) / 2.0f;

    // create indices
    float xVertex = corner;
    float zVertex = corner;
    for (int i = 0; i < chunkSize; i++) {
        for (int j = 0; j < chunkSize; j++) {
            Vertex tempVertex = {xVertex, distribution(engine), zVertex};
            mMesh.vertices.push_back(tempVertex);

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
    checkCollision();
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

void World::checkCollision() {
    glm::vec3 currentPos = mPlayer.getPosition();
    float playerHeight = mConfig.INTERNAL_SETTINGS.PLAYER_HEIGHT;
    int chunkSize = mConfig.INTERNAL_SETTINGS.CHUNK_SIZE;

    glm::vec3 nearest = castToVec3(mMesh.vertices[0]);
    int nearestIndex = 0;
    float minDistance = glm::distance(currentPos, nearest);

    for (int i = 0; i < mMesh.vertices.size(); i++) {
        Vertex vert = mMesh.vertices[i];
        glm::vec3 castVert = castToVec3(vert);
        float tempDistance = glm::distance(castVert, currentPos - glm::vec3(0.0, playerHeight, 0.0));
        if (tempDistance < minDistance) {
            minDistance = tempDistance;
            nearest = castVert;
            nearestIndex = i;
        }
    }

    // check for floor collision
    if (currentPos.y - playerHeight < nearest.y) {
        // interpolate between triangles
        glm::vec3 adjustedPos;
        // check if at edge of chunk
        if (nearestIndex < chunkSize || nearestIndex > chunkSize * (chunkSize - 1)
            || (nearestIndex % chunkSize) == (chunkSize - 1) || (nearestIndex % chunkSize) == 0) {
            adjustedPos.y = nearest.y;
        } else {
            if (currentPos.x > nearest.x) {
                glm::vec3 top = castToVec3(mMesh.vertices[nearestIndex - chunkSize]);
                if (currentPos.z > nearest.z) {
                    glm::vec3 right = castToVec3(mMesh.vertices[nearestIndex + 1]);
                    glm::vec3 topRight = castToVec3(mMesh.vertices[nearestIndex - chunkSize + 1]);
                    adjustedPos = collideQuad(currentPos, topRight, top, nearest, right);
                } else if (currentPos.z <= nearest.z) {
                    glm::vec3 left = castToVec3(mMesh.vertices[nearestIndex - 1]);
                    glm::vec3 topLeft = castToVec3(mMesh.vertices[nearestIndex - chunkSize - 1]);
                    adjustedPos = collideQuad(currentPos, top, topLeft, left, nearest);
                }
            } else if (currentPos.x <= nearest.x) {
                glm::vec3 bottom = castToVec3(mMesh.vertices[nearestIndex + chunkSize]);
                if (currentPos.z > nearest.z) {
                    glm::vec3 right = castToVec3(mMesh.vertices[nearestIndex + 1]);
                    glm::vec3 bottomRight = castToVec3(mMesh.vertices[nearestIndex + chunkSize - 1]);
                    adjustedPos = collideQuad(currentPos, right, nearest, bottom, bottomRight);
                } else if (currentPos.z <= nearest.z) {
                    glm::vec3 left = castToVec3(mMesh.vertices[nearestIndex - 1]);
                    glm::vec3 bottomLeft = castToVec3(mMesh.vertices[nearestIndex + chunkSize + 1]);
                    adjustedPos = collideQuad(currentPos, nearest, left, bottomLeft, bottom);
                }
            }
        }

        mPlayer.setPosition(glm::vec3(currentPos.x, adjustedPos.y + playerHeight, currentPos.z));
    }
}
