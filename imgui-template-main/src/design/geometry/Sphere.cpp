#include "Sphere.hpp"
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <vector>

Sphere::Sphere(int segments) {
    std::vector<ShapeVertex> vertices;
    
    for(int y = 0; y <= segments; ++y) {
        for(int x = 0; x <= segments; ++x) {
            float xSegment = (float)x / (float)segments;
            float ySegment = (float)y / (float)segments;
            
            
            float xPos = std::cos(xSegment * 2.0f * glm::pi<float>()) * std::sin(ySegment * glm::pi<float>());
            float yPos = std::cos(ySegment * glm::pi<float>());
            float zPos = std::sin(xSegment * 2.0f * glm::pi<float>()) * std::sin(ySegment * glm::pi<float>());

            ShapeVertex v;
            v.position = glm::vec3(xPos, yPos, zPos) * 0.5f; 
            v.normal = glm::normalize(glm::vec3(xPos, yPos, zPos));
            v.texCoords = glm::vec2(xSegment, ySegment);
            vertices.push_back(v);
        }
    }

    
    
    std::vector<ShapeVertex> finalVertices;
    for(int y = 0; y < segments; ++y) {
        for(int x = 0; x < segments; ++x) {
            finalVertices.push_back(vertices[(y + 1) * (segments + 1) + x]);
            finalVertices.push_back(vertices[y * (segments + 1) + x]);
            finalVertices.push_back(vertices[y * (segments + 1) + x + 1]);

            finalVertices.push_back(vertices[(y + 1) * (segments + 1) + x]);
            finalVertices.push_back(vertices[y * (segments + 1) + x + 1]);
            finalVertices.push_back(vertices[(y + 1) * (segments + 1) + x + 1]);
        }
    }
    m_vertexCount = finalVertices.size();

    glGenVertexArrays(1, &m_vao); glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo); glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, finalVertices.size() * sizeof(ShapeVertex), finalVertices.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void*)offsetof(ShapeVertex, position));
    glEnableVertexAttribArray(1); glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void*)offsetof(ShapeVertex, normal));
    glEnableVertexAttribArray(2); glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void*)offsetof(ShapeVertex, texCoords));
    glBindVertexArray(0);
}

Sphere::~Sphere() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
}

void Sphere::draw() const {
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    glBindVertexArray(0);
}