#include "Cube.hpp"

Cube::Cube() {
    std::vector<ShapeVertex> vertices; 
    
    float pts[] = {
        // Face Arrière
        -0.5f,-0.5f,-0.5f,    0, 0,-1,            0.0f, 0.0f,
         0.5f,-0.5f,-0.5f,    0, 0,-1,            1.0f, 0.0f,
         0.5f, 0.5f,-0.5f,    0, 0,-1,            1.0f, 1.0f,
         0.5f, 0.5f,-0.5f,    0, 0,-1,            1.0f, 1.0f,
        -0.5f, 0.5f,-0.5f,    0, 0,-1,            0.0f, 1.0f,
        -0.5f,-0.5f,-0.5f,    0, 0,-1,            0.0f, 0.0f,
        // Face Avant
        -0.5f,-0.5f, 0.5f,    0, 0, 1,            0.0f, 0.0f,
         0.5f,-0.5f, 0.5f,    0, 0, 1,            1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,    0, 0, 1,            1.0f, 1.0f,
         0.5f, 0.5f, 0.5f,    0, 0, 1,            1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f,    0, 0, 1,            0.0f, 1.0f,
        -0.5f,-0.5f, 0.5f,    0, 0, 1,            0.0f, 0.0f,
        // Face Gauche
        -0.5f, 0.5f, 0.5f,   -1, 0, 0,            1.0f, 0.0f,
        -0.5f, 0.5f,-0.5f,   -1, 0, 0,            1.0f, 1.0f,
        -0.5f,-0.5f,-0.5f,   -1, 0, 0,            0.0f, 1.0f,
        -0.5f,-0.5f,-0.5f,   -1, 0, 0,            0.0f, 1.0f,
        -0.5f,-0.5f, 0.5f,   -1, 0, 0,            0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,   -1, 0, 0,            1.0f, 0.0f,
        // Face Droite
         0.5f, 0.5f, 0.5f,    1, 0, 0,            0.0f, 0.0f,
         0.5f, 0.5f,-0.5f,    1, 0, 0,            0.0f, 1.0f,
         0.5f,-0.5f,-0.5f,    1, 0, 0,            1.0f, 1.0f,
         0.5f,-0.5f,-0.5f,    1, 0, 0,            1.0f, 1.0f,
         0.5f,-0.5f, 0.5f,    1, 0, 0,            1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,    1, 0, 0,            0.0f, 0.0f,
        // Face Dessous
        -0.5f,-0.5f,-0.5f,    0,-1, 0,            0.0f, 1.0f,
         0.5f,-0.5f,-0.5f,    0,-1, 0,            1.0f, 1.0f,
         0.5f,-0.5f, 0.5f,    0,-1, 0,            1.0f, 0.0f,
         0.5f,-0.5f, 0.5f,    0,-1, 0,            1.0f, 0.0f,
        -0.5f,-0.5f, 0.5f,    0,-1, 0,            0.0f, 0.0f,
        -0.5f,-0.5f,-0.5f,    0,-1, 0,            0.0f, 1.0f,
        // Face Dessus
        -0.5f, 0.5f,-0.5f,    0, 1, 0,            0.0f, 1.0f,
         0.5f, 0.5f,-0.5f,    0, 1, 0,            1.0f, 1.0f,
         0.5f, 0.5f, 0.5f,    0, 1, 0,            1.0f, 0.0f,
         0.5f, 0.5f, 0.5f,    0, 1, 0,            1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f,    0, 1, 0,            0.0f, 0.0f,
        -0.5f, 0.5f,-0.5f,    0, 1, 0,            0.0f, 1.0f
    };

    for(int i = 0; i < 36; i++) {
        ShapeVertex v;
        v.position = glm::vec3(pts[i*8], pts[i*8+1], pts[i*8+2]);
        v.normal = glm::vec3(pts[i*8+3], pts[i*8+4], pts[i*8+5]); 
        v.texCoords = glm::vec2(pts[i*8+6], pts[i*8+7]); // ON CHARGE LES UV ICI
        vertices.push_back(v);
    }

    glGenVertexArrays(1, &m_vao); glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo); glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(ShapeVertex), vertices.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, position));
    glEnableVertexAttribArray(1); glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, normal));
    glEnableVertexAttribArray(2); glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, texCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0); glBindVertexArray(0);
}

Cube::~Cube() { glDeleteBuffers(1, &m_vbo); glDeleteVertexArrays(1, &m_vao); }

void Cube::draw() const {
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}