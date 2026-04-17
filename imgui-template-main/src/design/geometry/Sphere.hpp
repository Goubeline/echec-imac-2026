#pragma once
#include "Shape.hpp"
#include <glad/glad.h>

class Sphere : public Shape {
private:
    GLuint m_vao;
    GLuint m_vbo;
    int m_vertexCount;
public:
    Sphere(int segments = 16);
    ~Sphere();
    void draw() const override;


};