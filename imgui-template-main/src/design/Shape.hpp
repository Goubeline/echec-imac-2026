#pragma once
#include <glm/glm.hpp>


struct ShapeVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Shape {
public:
    virtual ~Shape() = default;
    virtual void draw() const = 0; 
};