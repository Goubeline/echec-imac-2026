#pragma once
#include <vector>
#include <algorithm>
#include <memory>
#include "../core/Renderer.hpp"
#include "../geometry/Shape.hpp"
#include "../geometry/Cube.hpp"
#include "../core/Texture.hpp" 

class BoardRenderer : public Renderer {
    std::unique_ptr<Shape> m_cube;
    std::unique_ptr<Texture> m_texLight;
    std::unique_ptr<Texture> m_texDark;
    std::unique_ptr<Texture> m_texGrass;

public:
    BoardRenderer() : Renderer() {
        m_cube = std::make_unique<Cube>();
        m_texLight = std::make_unique<Texture>("../../assets/bois_clair.png");
        m_texDark  = std::make_unique<Texture>("../../assets/bois_fonce.png");
        m_texGrass = std::make_unique<Texture>("../../assets/grass.png");
    }

    void setLighting(const glm::mat4& view, float dayNightFactor) {
        applyLighting(view, dayNightFactor, 0.1f, 32.0f);
    }

    void render(const glm::mat4& view, const glm::mat4& proj, int selC, int selR, const std::vector<std::pair<int, int>>& moves, int hoverC, int hoverR) {
        m_program->use();

        // --- 1. HERBE  ---
        glm::mat4 groundModel = glm::scale(
            glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, -8.6f, 3.5f)), 
            glm::vec3(300.0f, 0.1f, 300.0f)
        );
        sendMatrices(proj * view * groundModel, view * groundModel);
        m_texGrass->bind(0);
        glUniform1i(m_program->uTexture, 0); 
        glUniform1i(m_program->uUseTexture, 1);
        glUniform1f(m_program->uUVScale, 10.0f); // Tiling herbe
        glUniform3f(m_program->uKd, 1.0f, 1.0f, 1.0f);
        m_cube->draw();

        // --- 2. TABLE  ---
        glUniform1f(m_program->uUVScale, 1.0f); // Reset Tiling
        glm::mat4 tableModel = glm::scale(
            glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, -4.5f, 3.5f)), 
            glm::vec3(15.0f, 8.0f, 15.0f)
        );
        sendMatrices(proj * view * tableModel, view * tableModel);
        m_texDark->bind(0);
        glUniform3f(m_program->uKd, 1.0f, 1.0f, 1.0f);
        m_cube->draw();

        // --- 3. BORDURE DU PLATEAU ---
        glm::mat4 borderModel = glm::scale(
            glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, -0.2f, 3.5f)),
            glm::vec3(8.5f, 0.4f, 8.5f)
        );
        sendMatrices(proj * view * borderModel, view * borderModel);
        m_texDark->bind(0);
        m_cube->draw();

        // --- 4. CASES DE L'ÉCHIQUIER ---
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                glm::mat4 m = glm::scale(
                    glm::translate(glm::mat4(1.0f), glm::vec3(c, 0.0f, r)),
                    glm::vec3(1.0f, 0.2f, 1.0f)
                );
                sendMatrices(proj * view * m, view * m);

                bool isPoss    = std::any_of(moves.begin(), moves.end(),
                                 [&](const auto& mv){ return mv.first==c && mv.second==r; });
                bool isHovered = (c == hoverC && r == hoverR);

                if (c == selC && r == selR) {
                    glUniform1i(m_program->uUseTexture, 0);
                    glUniform3f(m_program->uKd, 0.2f, 0.8f, 0.2f); // Sélectionné
                } else if (isHovered) {
                    glUniform1i(m_program->uUseTexture, 0);
                    glUniform3f(m_program->uKd, 0.8f, 0.8f, 0.2f); // Survol
                } else if (isPoss) {
                    glUniform1i(m_program->uUseTexture, 0);
                    glUniform3f(m_program->uKd, 0.4f, 0.9f, 0.4f); // Coup possible
                } else if ((r + c) % 2 == 0) {
                    m_texLight->bind(0);
                    glUniform1i(m_program->uTexture, 0);
                    glUniform1i(m_program->uUseTexture, 1);
                    glUniform3f(m_program->uKd, 1.0f, 1.0f, 1.0f);
                } else {
                    m_texDark->bind(0);
                    glUniform1i(m_program->uTexture, 0);
                    glUniform1i(m_program->uUseTexture, 1);
                    glUniform3f(m_program->uKd, 1.0f, 1.0f, 1.0f);
                }
                m_cube->draw();
            }
        }
        glUniform1i(m_program->uUseTexture, 0);
    }
};