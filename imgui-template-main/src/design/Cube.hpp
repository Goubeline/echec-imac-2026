#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>


struct ShapeVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Cube {
public:
    Cube();
    ~Cube();
    void draw();
    int getVertexCount() const { return 36; } // (td)

private:
    GLuint m_vao;
    GLuint m_vbo;
};