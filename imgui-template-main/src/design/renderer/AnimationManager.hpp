#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <cmath>
#include <optional>
#include <vector>
#include <memory>
#include "../../logic/piece.hpp"

struct Animation {
    glm::vec3 startPos;
    glm::vec3 endPos;
    float progress = 0.0f;
    float speed = 2.0f; 
    bool isFinished = false;
    int colDest, rowDest; 
};

class AnimationManager {
public:
    std::optional<Animation> m_currentAnim;

    // --- GESTION DU CIMETIÈRE ---
    std::vector<std::pair<std::unique_ptr<Piece>, glm::vec3>> graveyard;
    std::unique_ptr<Piece> flyingPiece;
    glm::vec3 flyStart, flyEnd;

    void start(glm::vec3 start, glm::vec3 end, int c, int r, std::unique_ptr<Piece> captured = nullptr) {
        
        if (flyingPiece) {
            graveyard.push_back({std::move(flyingPiece), flyEnd});
        }

        m_currentAnim = {start, end, 0.0f, 2.0f, false, c, r};
        
        // Si une pièce a été mangée à ce tour :
        flyingPiece = std::move(captured);
        if (flyingPiece) {
            flyStart = end; // Elle est frappée sur sa case (Y = 0.0)
            
            // Les B volent à gauche (X = -2), les N à droite (X = 9)
            float sideX = flyingPiece->i_am_white ? -2.0f : 9.0f;
            float sideZ = (graveyard.size() % 8) * 1.0f; 
            
            
            flyEnd = glm::vec3(sideX, -0.4f, sideZ);
        }
    }

    void update(float deltaTime) {
        if (!m_currentAnim) return;
        m_currentAnim->progress += deltaTime * m_currentAnim->speed;
        if (m_currentAnim->progress >= 1.0f) {
            m_currentAnim->progress = 1.0f;
            m_currentAnim->isFinished = true;
        }
    }

    bool isCellAnimating(int c, int r) const {
        return m_currentAnim && m_currentAnim->colDest == c && m_currentAnim->rowDest == r && !m_currentAnim->isFinished;
    }

    // Trajectoire de l'attaquant 
    glm::vec3 getCurrentPosition() const {
        if (!m_currentAnim) return glm::vec3(0);
        float t = m_currentAnim->progress;
        float smoothT = t * t * (3.0f - 2.0f * t);
        glm::vec3 pos = glm::mix(m_currentAnim->startPos, m_currentAnim->endPos, smoothT);
        pos.y += std::sin(t * glm::pi<float>()) * 1.8f; // Cloche
        return pos;
    }

    // Trajectoire du défenseur
    glm::vec3 getFlyingPosition() const {
        if (!m_currentAnim || !flyingPiece) return glm::vec3(0);
        float t = m_currentAnim->progress;
        float smoothT = t * t * (3.0f - 2.0f * t);
        glm::vec3 pos = glm::mix(flyStart, flyEnd, smoothT);
       
        pos.y += std::sin(t * glm::pi<float>()) * 3.0f; 
        return pos;
    }
};