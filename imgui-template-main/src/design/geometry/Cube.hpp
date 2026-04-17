#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shape.hpp"

class Cube : public Shape {

private:
    GLuint m_vao;
    GLuint m_vbo;
    
public:
    Cube();
    ~Cube();
    void draw() const override;
    int getVertexCount() const { return 36; }

};