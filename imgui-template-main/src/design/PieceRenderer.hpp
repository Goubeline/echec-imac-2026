#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include "Shape.hpp"
#include "Cube.hpp"
#include "Sphere.hpp" 
#include "Programs.hpp"
#include "AnimationManager.hpp"
#include "../logic/chessboard.hpp"
#include "../logic/pawn.hpp"
#include "../logic/rook.hpp"
#include "../logic/knight.hpp"
#include "../logic/bishop.hpp"
#include "../logic/queen.hpp"
#include "../logic/king.hpp"

class PieceRenderer {
    std::unique_ptr<BoardProgram> m_program;
    
    
    std::unique_ptr<Shape> m_cube;
    std::unique_ptr<Shape> m_sphere;

public:
    PieceRenderer() {
        m_program = std::make_unique<BoardProgram>();
        m_cube = std::make_unique<Cube>();
        m_sphere = std::make_unique<Sphere>();
    }

    void render(Chessboard& board, const glm::mat4& view, const glm::mat4& proj, const AnimationManager& anim) {
        m_program->use();
        auto& grid = board.get_board();

        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                if (grid[c][r]) {
                    Piece* p = grid[c][r].get();
                    glm::vec3 pPos = anim.isCellAnimating(c, r) ? anim.getCurrentPosition() : glm::vec3(c, 0.0f, r);
                    
                    if (p->i_am_white) glUniform3f(m_program->uColor, 0.9f, 0.8f, 0.5f);
                    else glUniform3f(m_program->uColor, 0.2f, 0.2f, 0.2f);

                    // --- SPHERE SUR LE PION ---
                    if (dynamic_cast<Pawn*>(p)) {
                        // Un corps en cube
                        glm::mat4 pm = glm::scale(glm::translate(glm::mat4(1.0f), pPos + glm::vec3(0, 0.4f, 0)), glm::vec3(0.4f, 0.4f, 0.4f));
                        sendMatrices(proj * view * pm, view * pm); m_cube->draw();
                        
                        // Une tête en sphère
                        pm = glm::scale(glm::translate(glm::mat4(1.0f), pPos + glm::vec3(0, 0.8f, 0)), glm::vec3(0.4f, 0.4f, 0.4f));
                        sendMatrices(proj * view * pm, view * pm); m_sphere->draw(); 
                    }
                    else {
                        // autre
                        glm::mat4 pm = glm::scale(glm::translate(glm::mat4(1.0f), pPos + glm::vec3(0, 0.5f, 0)), glm::vec3(0.5f, 0.8f, 0.5f));
                        sendMatrices(proj * view * pm, view * pm); m_cube->draw();
                    }
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