#pragma once

#include <glm/glm.hpp>

struct Entity {
    Entity()
        : box(glm::vec3(0.0f)), position(glm::vec3(0.f)), rotation(glm::vec3(0.f)), velocity(glm::vec3(0.f))
    {}

    Entity(const glm::vec3 &pos)
        : position(pos), rotation(glm::vec3(0.0f)), box({0, 0, 0}), velocity(glm::vec3(0.f))
    {

    }

    Entity(const glm::vec3 &pos, const glm::vec3 &rot)
        : position(pos), rotation(rot), box(glm::vec3(0.0f)), velocity(glm::vec3(0.f))
    {

    }

    Entity(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &box)
        : position(pos), rotation(rot), box(box), velocity(glm::vec3(0.f))
    {

    }

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 velocity;
    glm::vec3 box;
};