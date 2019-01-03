#pragma once

#include <glm/glm.hpp>

class Entity {
public:
    Entity()
            : box(glm::vec3(0.0f)), position(glm::vec3(0.0f)), rotation(glm::vec3(0.0f)), velocity(glm::vec3(0.0f))
    {}

    Entity(const glm::vec3 &pos)
            : position(pos), rotation(glm::vec3(0.0f)), box(glm::vec3(0.0f)), velocity(glm::vec3(0.0f))
    {}

    Entity(const glm::vec3 &pos, const glm::vec3 &rot)
            : position(pos), rotation(rot), box(glm::vec3(0.0f)), velocity(glm::vec3(0.0f))
    {}

    Entity(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &box)
            : position(pos), rotation(rot), box(box), velocity(glm::vec3(0.0f))
    {}

    const glm::vec3 &getPosition() const
    {
        return position;
    }

    void setPosition(const glm::vec3 &position)
    {
        Entity::position = position;
    }

    const glm::vec3 &getRotation() const
    {
        return rotation;
    }

    void setRotation(const glm::vec3 &rotation)
    {
        Entity::rotation = rotation;
    }

    const glm::vec3 &getVelocity() const
    {
        return velocity;
    }

    void setVelocity(const glm::vec3 &velocity)
    {
        Entity::velocity = velocity;
    }

    const glm::vec3 &getBox() const
    {
        return box;
    }

    void setBox(const glm::vec3 &box)
    {
        Entity::box = box;
    }

protected:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 velocity;
    glm::vec3 box;
};