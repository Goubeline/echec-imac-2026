#pragma once
#include <memory>
#include "../core/Renderer.hpp"
#include "../core/Texture.hpp" // Retour à Texture classique
#include "../core/Programs.hpp"

class SkyboxRenderer {
    std::unique_ptr<SkyboxProgram> m_program;
    std::unique_ptr<Texture> m_texture;
    GLuint m_vao, m_vbo;

public:
    SkyboxRenderer() {
        m_program = std::make_unique<SkyboxProgram>();
        
        // On reprend ton image sky.jpg simple
        m_texture = std::make_unique<Texture>("../../assets/sky.jpg"); 

        // Les anciens vertices avec les coordonnées de texture (X,Y,Z,  U,V)
        float skyboxVertices[] = {
            -1,-1,-1, 0,0,  1,-1,-1, 1,0,  1, 1,-1, 1,1,  1, 1,-1, 1,1, -1, 1,-1, 0,1, -1,-1,-1, 0,0,
            -1,-1, 1, 0,0,  1,-1, 1, 1,0,  1, 1, 1, 1,1,  1, 1, 1, 1,1, -1, 1, 1, 0,1, -1,-1, 1, 0,0,
            -1, 1, 1, 1,1, -1, 1,-1, 0,1, -1,-1,-1, 0,0, -1,-1,-1, 0,0, -1,-1, 1, 1,0, -1, 1, 1, 1,1,
             1, 1, 1, 0,1,  1, 1,-1, 1,1,  1,-1,-1, 1,0,  1,-1,-1, 1,0,  1,-1, 1, 0,0,  1, 1, 1, 0,1,
            -1,-1,-1, 0,1,  1,-1,-1, 1,1,  1,-1, 1, 1,0,  1,-1, 1, 1,0, -1,-1, 1, 0,0, -1,-1,-1, 0,1,
            -1, 1,-1, 0,1,  1, 1,-1, 1,1,  1, 1, 1, 1,0,  1, 1, 1, 1,0, -1, 1, 1, 0,0, -1, 1,-1, 0,1
        };

        glGenVertexArrays(1, &m_vao); 
        glGenBuffers(1, &m_vbo);
        glBindVertexArray(m_vao); 
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0); 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
        
        glEnableVertexAttribArray(2); // Les UV sont sur la location 2
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    }

    ~SkyboxRenderer() {
        glDeleteBuffers(1, &m_vbo);
        glDeleteVertexArrays(1, &m_vao);
    }

    void render(const glm::mat4& view, const glm::mat4& proj, float factor) {
        m_program->use();
        
        glm::mat4 skyView = glm::mat4(glm::mat3(view)); 
        glm::mat4 mvp = proj * skyView * glm::scale(glm::mat4(1.0f), glm::vec3(100.0f));
        
        glUniformMatrix4fv(m_program->uMVP, 1, GL_FALSE, glm::value_ptr(mvp));
        glUniform1f(m_program->uDayNightFactor, factor);
        
        m_texture->bind(0);
        glUniform1i(m_program->uTexture, 0);

        glDepthMask(GL_FALSE);
        glBindVertexArray(m_vao); 
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);
    }
};