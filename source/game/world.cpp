#include "world.h"

#include "config.h"
#include "perlin.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <string>

#include <random>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float tempHeight = 1;
bool bottomLeftBool = true;
float delta = 0.0f;

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
        : shader("res/shaders/checker.vert", "res/shaders/checker.frag"),
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

    // wireframe
//     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    shader.use();
}

void World::generateWorld()
{
    int imageX, imageY, n;
    unsigned char *data = stbi_load("../test.png", &imageX, &imageY, &n, 0);
    // ... process data if not NULL ...
    // ... x = width, y = height, n = # 8-bit components per pixel ...
    // ... replace '0' with '1'..'4' to force that many components per pixel
    // ... but 'n' will always be the number that it would have been if you said 0
    mapSize = imageX;
    for (int i = 0; i < imageX * imageY; i++) {
//        printf("%d\n", data[i]);
        mapData.push_back((float) data[i] / 255.0f);
    }

    stbi_image_free(data);

    glm::vec3 pos = mPlayer.getPosition();
    float xPos = floor(pos.x);
    float zPos = floor(pos.z);

    std::default_random_engine engine;
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    unsigned int time_ui = static_cast<unsigned int>( time(NULL) );

    PerlinNoise noise(20);

    int chunkSize = mConfig.INTERNAL_SETTINGS.CHUNK_SIZE;

    // make sure chunk size is odd
    if (chunkSize % 2 == 0) {
        chunkSize++;
    }

    int noiseSize = 171;

    // distance from center to edge (absolute value of a single corner value)
    float corner = (chunkSize - 1) / 2.0f;

    // create vertices
    // x goes from - to + (left to right)
    // z goes from + to - (front to back)
    // (-1,  1), (0,  1), (1,  1)
    // (-1,  0), (0,  0), (1,  0)
    // (-1, -1), (0, -1), (1, -1)
    for (int z = corner; z >= -corner; z--) {
        for (int x = -corner; x <= corner; x++) {
            Vertex tempVertex = {float(x), 0, float(z)};
            mMesh.vertices.push_back(tempVertex);
        }
    }

    // create triangles from vertices
    // creates bottom left and top right; for example
    //     0           1           2
    // (-1,  1), -- (0,  1), -- (1,  1)
    //     |   \      |   \     |
    //     |    \     |    \    |
    //     |     \    |     \   |
    //     |      \   |      \  |
    //     3           4           5
    // (-1,  0), -- (0,  0), -- (1,  0)
    //     |   \      |   \     |
    //     |    \     |    \    |
    //     |     \    |     \   |
    //     |      \   |      \  |
    //     6           7           8
    // (-1, -1), -- (0, -1), -- (1, -1)
    mMesh.indices.clear();
    for (unsigned int i = 0; i < chunkSize * (chunkSize - 1); i += chunkSize) {
        for (unsigned int j = 0; j < chunkSize - 1; j++) {
            mMesh.indices.push_back(Index{i + j, i + j + chunkSize, i + j + chunkSize + 1});
            mMesh.indices.push_back(Index{i + j, i + j + 1, i + j + chunkSize + 1});
        }
    }

}

float World::getMapHeight(int x, int y) {
    int center = (mapSize / 2) * mapSize + mapSize / 2;
    if (x <= -mapSize / 2 || y <= -mapSize / 2 || x >= mapSize / 2 || y >= mapSize / 2) {
        return 0.0f;
    }
    return mapData[center - mapSize * x + y];
}

void World::update(float deltaTime)
{
    delta = deltaTime;

    int chunkSize = mConfig.INTERNAL_SETTINGS.CHUNK_SIZE;
    int noiseSize = 171;
    PerlinNoise noise(20);
    float time = glm::cos(glm::sin((float) glfwGetTime()));

    glm::vec3 playerPos = mPlayer.getPosition();
    float xPos = floor(playerPos.x);
    float zPos = floor(playerPos.z);

    // world updating
    float step = 1.0f;

    float corner = (chunkSize - 1) / 2.0f;

    // create vertices
    float xVertex = -corner;
    float zVertex = corner;
    for (int i = 0; i < chunkSize; i++) {
        for (int j = 0; j < chunkSize; j++) {
//            float height = (float) noise.octaveNoise0_1((xVertex + xPos) * 10 / 171, (zVertex + zPos) * 10 / 171, time, 1) * 4;
            float height = getMapHeight(xVertex + xPos, zVertex + zPos) * 10.0f;
            mMesh.vertices[i * chunkSize + j] = {xVertex + xPos, height, zVertex + zPos};

            xVertex += 1.0f;
        }
        zVertex -= 1.0f;
        xVertex = -corner;
    }

    glBufferData(GL_ARRAY_BUFFER, mMesh.vertices.size() * sizeof(Vertex), mMesh.vertices.data(), GL_STATIC_DRAW);
    checkCollision();
}


void World::render(float framerate, const glm::mat4 proj, const glm::mat4 view, bool menu)
{
    glm::vec3 skyColor = {171, 212, 220};
    skyColor /= 255;

    // clear window background and buffer
    glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set shader variables here
    shader.setMat4("Projection", proj);
    shader.setMat4("View", view);
    shader.setVec3("LightAngle", glm::vec3(0.0, -1.0, 0.0));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (int) mMesh.indices.size() * 3, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (mPlayer.isMenu()) {
        ImGui::Begin("Another Window");   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        ImGui::End();
    }

    // draw reticle
    float reticleDiameter = 6.f;
    ImVec2 _reticleCenter = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImVec2 _reticleWindowSize = ImVec2(reticleDiameter, reticleDiameter);
    ImVec2 _reticleWindowPosition = ImVec2(_reticleCenter.x - _reticleWindowSize.x * 0.5f,_reticleCenter.y -  _reticleWindowSize.y * 0.5f);

    float _reticleThickness = 1.f;
    float _reticleRadiusOutline = reticleDiameter * 0.5f;
    float _reticleRadius = _reticleRadiusOutline - _reticleThickness;
    ImU32 _reticleColorOutline = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 1.f));
    ImU32 _reticleColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f, 1.f));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(1.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.1f);

    ImGui::SetNextWindowPos(_reticleWindowPosition);
    ImGui::SetNextWindowSize(_reticleWindowSize);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::Begin("reticle", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoInputs);
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddCircleFilled(_reticleCenter, _reticleRadius, _reticleColor, 12);
    drawList->AddCircle(_reticleCenter, _reticleRadiusOutline, _reticleColorOutline, 12, _reticleThickness);

    ImGui::PopStyleVar(5);

    ImGui::End();

    ImGui::Begin("Debug", nullptr,  ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text(("height: " + std::to_string(tempHeight)).c_str());
    ImGui::Text(bottomLeftBool ? "True" : "False");
    ImGui::Text(("x: " + std::to_string(mPlayer.getPosition().x)).c_str());
    ImGui::Text(("z: " + std::to_string(mPlayer.getPosition().z)).c_str());
    ImGui::Text(("fps: " + std::to_string(framerate)).c_str());
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(mPlayer.getWindow());
}

void World::checkCollision()
{
    float playerHeight = mConfig.INTERNAL_SETTINGS.PLAYER_HEIGHT;
    int chunkSize = mConfig.INTERNAL_SETTINGS.CHUNK_SIZE;

    glm::vec3 currentPos = mPlayer.getPosition();

    // remember: y would be equal to z
    glm::vec2 currentPos2D = castToVec2(mPlayer.getPosition());

    // build offset triangle -- offset from center
    glm::vec2 triangle[3] = {
            {0, 1},
            {1,  0},
    };

    glm::vec2 bottomLeft = {glm::floor(currentPos.x), glm::floor(currentPos.z)};
    glm::vec2 topRight = {glm::ceil(currentPos.x), glm::ceil(currentPos.z)};

    if (glm::distance(currentPos2D, bottomLeft) < glm::distance(currentPos2D, topRight)) {
        triangle[2] = {0,  0};
        bottomLeftBool = true;
    } else {
        triangle[2] = {1,  1};
        bottomLeftBool = false;
    }

    glm::vec3 height[3];

    int centerIndex = chunkSize / 2 + chunkSize * (chunkSize / 2);

    // values other than y are for debugging -- remove later
    for (int i = 0; i < 3; i++) {
        int index = centerIndex + (int) triangle[i].x - (int) triangle[i].y * chunkSize;
        height[i] = {mMesh.vertices[index].positions[0], mMesh.vertices[index].positions[1],
                     mMesh.vertices[index].positions[2]};
    }

    glm::vec2 adjustedTriangle[3] = {
            glm::floor(currentPos2D) + triangle[0],
            glm::floor(currentPos2D) + triangle[1],
            glm::floor(currentPos2D) + triangle[2],
    };

    float u, v, w;
    barycentric(currentPos2D, adjustedTriangle[0], adjustedTriangle[1], adjustedTriangle[2], u, v, w);
    tempHeight = u * height[0].y + v * height[1].y + w * height[2].y;

    if (!mConfig.INTERNAL_SETTINGS.FLY)
        mPlayer.setPosition(glm::vec3(currentPos2D.x, tempHeight + playerHeight, currentPos2D.y));
}
