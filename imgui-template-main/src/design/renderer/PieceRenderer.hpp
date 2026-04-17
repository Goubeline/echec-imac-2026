#pragma once
#include <memory>
// Nouveaux chemins
#include "../core/Renderer.hpp"
#include "../geometry/Sphere.hpp"
#include "../geometry/Cube.hpp"
#include "../core/Texture.hpp"
#include "AnimationManager.hpp"


#include "../../logic/chessboard.hpp"
#include "../../logic/pawn.hpp"
#include "../../logic/rook.hpp"
#include "../../logic/knight.hpp"
#include "../../logic/bishop.hpp"
#include "../../logic/queen.hpp"
#include "../../logic/king.hpp"

class PieceRenderer : public Renderer {
    std::unique_ptr<Shape> m_cube;

private:
    void drawSinglePiece(Piece* p, glm::vec3 pPos, const glm::mat4& view, const glm::mat4& proj, float angle = 0.0f) {
      
        


        if (p->i_am_white) glUniform3f(m_program->uKd, 0.85f, 0.60f, 0.40f); 
        else glUniform3f(m_program->uKd, 0.20f, 0.18f, 0.15f);

        glm::mat4 model = glm::translate(glm::mat4(1.0f), pPos);
        if (angle != 0.0f) {
            model = glm::translate(model, glm::vec3(0, 0.3f, 0)); 
            model = glm::rotate(model, angle, glm::vec3(0.8f, 0.0f, 0.5f)); 
            model = glm::translate(model, glm::vec3(0, -0.3f, 0));
        }

        // ==========================================
        // 1. BASE 
        // ==========================================
        // Le socle principal
        glm::mat4 base = glm::scale(glm::translate(model, glm::vec3(0, 0.075f, 0)), glm::vec3(0.5f, 0.15f, 0.5f));
        sendMatrices(proj * view * base, view * base); m_cube->draw();
        
        // La "Fente"
        glm::mat4 groove = glm::scale(glm::translate(model, glm::vec3(0, 0.175f, 0)), glm::vec3(0.4f, 0.05f, 0.4f));
        sendMatrices(proj * view * groove, view * groove); m_cube->draw();

        float startY = 0.20f; // Point de départ pour le corps des pièces
        float bodyW = 0.45f;  // Largeur du corps 

        // ==========================================
        // 2. CORPS DES PIÈCES SPÉCIFIQUES
        // ==========================================
        if (dynamic_cast<Pawn*>(p)) {
            // Pion 
            glm::mat4 body = glm::scale(glm::translate(model, glm::vec3(0, startY + 0.15f, 0)), glm::vec3(bodyW, 0.3f, bodyW));
            sendMatrices(proj * view * body, view * body); m_cube->draw();
        } 
        else if (dynamic_cast<Rook*>(p)) {
            // Tour : Bloc plus haut + 2ème fente + "Chapeau"
            glm::mat4 body = glm::scale(glm::translate(model, glm::vec3(0, startY + 0.2f, 0)), glm::vec3(bodyW, 0.4f, bodyW));
            sendMatrices(proj * view * body, view * body); m_cube->draw();
            
            glm::mat4 groove2 = glm::scale(glm::translate(model, glm::vec3(0, startY + 0.425f, 0)), glm::vec3(0.4f, 0.05f, 0.4f));
            sendMatrices(proj * view * groove2, view * groove2); m_cube->draw();
            
            glm::mat4 top = glm::scale(glm::translate(model, glm::vec3(0, startY + 0.55f, 0)), glm::vec3(0.5f, 0.2f, 0.5f));
            sendMatrices(proj * view * top, view * top); m_cube->draw();
        } 
        else if (dynamic_cast<Knight*>(p)) {
            // Cavalier : Le corps droit + une tête penchée 
            float dir = p->i_am_white ? 1.0f : -1.0f;
            
            glm::mat4 body = glm::scale(glm::translate(model, glm::vec3(0, startY + 0.2f, 0)), glm::vec3(bodyW, 0.4f, bodyW));
            sendMatrices(proj * view * body, view * body); m_cube->draw();
            
            // Fente du cou
            glm::mat4 groove2 = glm::scale(glm::translate(model, glm::vec3(0, startY + 0.425f, 0)), glm::vec3(0.4f, 0.05f, 0.4f));
            sendMatrices(proj * view * groove2, view * groove2); m_cube->draw();

            // Tête en Rotation
            glm::mat4 head = glm::translate(model, glm::vec3(0.0f, startY + 0.6f, 0.05f * dir));
            head = glm::rotate(head, glm::radians(35.0f * dir), glm::vec3(1.0f, 0.0f, 0.0f));
            head = glm::scale(head, glm::vec3(bodyW, 0.4f, bodyW));
            sendMatrices(proj * view * head, view * head); m_cube->draw();
        } 
        else if (dynamic_cast<Bishop*>(p)) {
            // Fou
            glm::mat4 body = glm::scale(glm::translate(model, glm::vec3(0, startY + 0.25f, 0)), glm::vec3(bodyW, 0.5f, bodyW));
            sendMatrices(proj * view * body, view * body); m_cube->draw();
            
            glm::mat4 groove2 = glm::scale(glm::translate(model, glm::vec3(0, startY + 0.525f, 0)), glm::vec3(0.4f, 0.05f, 0.4f));
            sendMatrices(proj * view * groove2, view * groove2); m_cube->draw();

            // Pointe 
            glm::mat4 head = glm::translate(model, glm::vec3(0.0f, startY + 0.7f, 0.0f));
            
            head = glm::scale(head, glm::vec3(0.35f, 0.4f, 0.35f));
            sendMatrices(proj * view * head, view * head); m_cube->draw();
        } 
        else if (dynamic_cast<Queen*>(p)) {
            // Dame 
            glm::mat4 body = glm::scale(glm::translate(model, glm::vec3(0, startY + 0.3f, 0)), glm::vec3(bodyW, 0.6f, bodyW));
            sendMatrices(proj * view * body, view * body); m_cube->draw();
            
            glm::mat4 groove2 = glm::scale(glm::translate(model, glm::vec3(0, startY + 0.625f, 0)), glm::vec3(0.4f, 0.05f, 0.4f));
            sendMatrices(proj * view * groove2, view * groove2); m_cube->draw();
            
            glm::mat4 top1 = glm::scale(glm::translate(model, glm::vec3(0, startY + 0.75f, 0)), glm::vec3(bodyW, 0.2f, bodyW));
            sendMatrices(proj * view * top1, view * top1); m_cube->draw();

            // Petit bloc du sommet
            glm::mat4 top2 = glm::scale(glm::translate(model, glm::vec3(0, startY + 0.875f, 0)), glm::vec3(0.15f, 0.05f, 0.15f));
            sendMatrices(proj * view * top2, view * top2); m_cube->draw();
        } 
        else if (dynamic_cast<King*>(p)) {
            // Roi 
            glm::mat4 body = glm::scale(glm::translate(model, glm::vec3(0, startY + 0.35f, 0)), glm::vec3(bodyW, 0.7f, bodyW));
            sendMatrices(proj * view * body, view * body); m_cube->draw();
            
            glm::mat4 groove2 = glm::scale(glm::translate(model, glm::vec3(0, startY + 0.725f, 0)), glm::vec3(0.4f, 0.05f, 0.4f));
            sendMatrices(proj * view * groove2, view * groove2); m_cube->draw();
            
            glm::mat4 top1 = glm::scale(glm::translate(model, glm::vec3(0, startY + 0.85f, 0)), glm::vec3(bodyW, 0.2f, bodyW));
            sendMatrices(proj * view * top1, view * top1); m_cube->draw();
            
         
            glm::mat4 c1 = glm::scale(glm::translate(model, glm::vec3(0, startY + 1.05f, 0)), glm::vec3(0.1f, 0.2f, 0.1f));
            sendMatrices(proj * view * c1, view * c1); m_cube->draw();
            glm::mat4 c2 = glm::scale(glm::translate(model, glm::vec3(0, startY + 1.05f, 0)), glm::vec3(0.3f, 0.08f, 0.1f));
            sendMatrices(proj * view * c2, view * c2); m_cube->draw();
        }
    }

public:
    PieceRenderer() : Renderer() {
        m_cube = std::make_unique<Cube>();
       
    }

    void setLighting(const glm::mat4& view, float dayNightFactor) {
        applyLighting(view, dayNightFactor, 0.15f, 8.0f);
    }

   void render(Chessboard& board, const glm::mat4& view, const glm::mat4& proj, const AnimationManager& anim) {
        m_program->use();
        auto& grid = board.get_board();
        
        for (int r = 0; r < 8; ++r) {
            for (int c = 0; c < 8; ++c) {
                if (grid[c][r]) {
                    Piece* p = grid[c][r].get();
                    glm::vec3 pPos = anim.isCellAnimating(c, r) ? anim.getCurrentPosition() : glm::vec3(c, 0.0f, r);
                    drawSinglePiece(p, pPos, view, proj);
                }
            }
        }

        if (anim.flyingPiece) {
            float rollAngle = anim.m_currentAnim->progress * glm::pi<float>() * 4.0f; 
            drawSinglePiece(anim.flyingPiece.get(), anim.getFlyingPosition(), view, proj, rollAngle);
        }

        for (const auto& grave : anim.graveyard) {
          
            drawSinglePiece(grave.first.get(), grave.second, view, proj, 0.0f); 
        }
    
    }
};