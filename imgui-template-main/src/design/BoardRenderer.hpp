#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <memory>
#include "Cube.hpp"
#include "Programs.hpp"
#include "AnimationManager.hpp"
#include "../logic/chessboard.hpp"

class BoardRenderer {
    std::unique_ptr<Cube> m_cube;
    std::unique_ptr<BoardProgram> m_program;

public:
    BoardRenderer() {
        m_cube = std::make_unique<Cube>();
        m_program = std::make_unique<BoardProgram>();
    }


    void render(Chessboard& board, const glm::mat4& view, const glm::mat4& proj, 
                int selC, int selR, const std::vector<std::pair<int, int>>& moves, 
                const AnimationManager& anim) {
        m_program->use();
        auto& grid = board.get_board();

        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                // Rendu de la Case
                glm::mat4 m = glm::scale(glm::translate(glm::mat4(1.0f), glm::vec3(c, 0.0f, r)), glm::vec3(1.0f, 0.2f, 1.0f));
                sendMatrices(proj * view * m, view * m);

                bool isPoss = std::any_of(moves.begin(), moves.end(), [&](auto& mv) { return mv.first == c && mv.second == r; });

                if (c == selC && r == selR) glUniform3f(m_program->uColor, 0.2f, 0.8f, 0.2f);
                else if (isPoss) glUniform3f(m_program->uColor, 0.4f, 0.9f, 0.4f);
                else if ((r + c) % 2 == 0) glUniform3f(m_program->uColor, 0.9f, 0.9f, 0.9f);
                else glUniform3f(m_program->uColor, 0.3f, 0.3f, 0.3f);
                m_cube->draw();

                // Rendu de la Pièce
                if (grid[c][r]) {
                    glm::vec3 pPos = anim.isCellAnimating(c, r) ? anim.getCurrentPosition() : glm::vec3(c, 0.5f, r);
                    glm::mat4 pm = glm::scale(glm::translate(glm::mat4(1.0f), pPos), glm::vec3(0.5f, 0.8f, 0.5f));
                    sendMatrices(proj * view * pm, view * pm);
                    if (grid[c][r]->i_am_white) glUniform3f(m_program->uColor, 0.8f, 0.7f, 0.2f);
                    else glUniform3f(m_program->uColor, 0.5f, 0.1f, 0.1f);
                    m_cube->draw();
                }
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