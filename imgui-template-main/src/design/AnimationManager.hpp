#pragma once
#include <glm/glm.hpp>
#include <optional>

struct Animation {
    glm::vec3 startPos;
    glm::vec3 endPos;
    float progress = 0.0f;
    float speed = 3.0f;
    bool isFinished = false;
    
    // On stocke quelle pièce bouge pour l'identifier
    int colDest, rowDest; 
};

class AnimationManager {
private:
    std::optional<Animation> m_currentAnim;

public:
    // Lancer une nouvelle animation
    void start(glm::vec3 start, glm::vec3 end, int c, int r) {
        m_currentAnim = {start, end, 0.0f, 3.0f, false, c, r};
    }

    // Mettre à jour le temps (à appeler dans la boucle)
    void update(float deltaTime) {
        if (!m_currentAnim) return;

        m_currentAnim->progress += deltaTime * m_currentAnim->speed;
        if (m_currentAnim->progress >= 1.0f) {
            m_currentAnim->progress = 1.0f;
            m_currentAnim->isFinished = true;
        }
    }

    // Savoir si une case est en train d'être animée 
    bool isCellAnimating(int c, int r) const {
        return m_currentAnim && m_currentAnim->colDest == c && m_currentAnim->rowDest == r && !m_currentAnim->isFinished;
    }

    // Récupérer la position actuelle calculée
    glm::vec3 getCurrentPosition() const {
        if (!m_currentAnim) return glm::vec3(0);
        // On utilise mix pour "l'interpolation linéaire" sinon erreur
        return glm::mix(m_currentAnim->startPos, m_currentAnim->endPos, m_currentAnim->progress);
    }

    bool hasFinished() const { return m_currentAnim && m_currentAnim->isFinished; }
    void clear() { m_currentAnim = std::nullopt; }
};