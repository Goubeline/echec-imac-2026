#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>
#include <algorithm>
#include "Shape.hpp"
#include "Cube.hpp"
#include "Programs.hpp"

class BoardRenderer {
    std::unique_ptr<Shape> m_cube;
    std::unique_ptr<BoardProgram> m_program;

public:
    BoardRenderer() {
        m_cube = std::make_unique<Cube>();
        m_program = std::make_unique<BoardProgram>();
    }

    void render(const glm::mat4& view, const glm::mat4& proj, int selC, int selR, const std::vector<std::pair<int, int>>& moves, int hoverC, int hoverR) {
        m_program->use();

        // 1. LE BORD DE L'ÉCHIQUIER
        glm::mat4 borderModel = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, -0.2f, 3.5f)), glm::vec3(8.5f, 0.4f, 8.5f));
        sendMatrices(proj * view * borderModel, view * borderModel);
        glUniform3f(m_program->uColor, 0.3f, 0.2f, 0.1f);
        m_cube->draw();

        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                // 2. RENDU DE LA CASE
                glm::mat4 m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(c, 0.0f, r)), glm::vec3(1.0f, 0.2f, 1.0f));
                sendMatrices(proj * view * m, view * m);

                bool isPoss = std::any_of(moves.begin(), moves.end(), [&](auto& mv) { return mv.first == c && mv.second == r; });
                bool isHovered = (c == hoverC && r == hoverR);

                if (c == selC && r == selR) glUniform3f(m_program->uColor, 0.2f, 0.8f, 0.2f);
                else if (isHovered) glUniform3f(m_program->uColor, 0.8f, 0.8f, 0.2f);
                else if (isPoss) glUniform3f(m_program->uColor, 0.4f, 0.9f, 0.4f);
                else if ((r + c) % 2 == 0) glUniform3f(m_program->uColor, 0.9f, 0.9f, 0.9f);
                else glUniform3f(m_program->uColor, 0.3f, 0.3f, 0.3f);
                
                m_cube->draw();
            }
        }
    }

private:
    void sendMatrices(const glm::mat4& mvp, const glm::mat4& mv) {
        glUniformMatrix4fv(m_program->uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvp));
        glUniformMatrix4fv(m_program->uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv));
        glUniformMatrix4fv(m_program->uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mv))));
    }
};