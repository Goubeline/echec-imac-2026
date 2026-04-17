#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class PieceCamera {
private:
    glm::vec3 m_Position; 
    float m_fAngleX;      
    float m_fAngleY;     

public:
    PieceCamera() {
        m_Position = glm::vec3(0.0f);
        m_fAngleX = 0.0f;   
        m_fAngleY = -90.0f; 
    }

  
    void setPosition(glm::vec3 pos) {
        m_Position = pos;
    }

    // "Tourner la tête" à gauche/droite
    void rotateLeft(float degrees) {
        m_fAngleY += degrees;
    }

    // Lever/Baisser la tête
    void rotateUp(float degrees) {
        m_fAngleX -= degrees;
        // On bloque à 89 degrés pour éviter que la caméra ne se retourne 
        if(m_fAngleX < -89.0f) m_fAngleX = -89.0f;
        if(m_fAngleX > 89.0f) m_fAngleX = 89.0f;
    }

    //  matrice de Vue
    glm::mat4 getViewMatrix() const {
        // Calcul du vecteur de direction grâce à la trigonométrie
        glm::vec3 front;
        front.x = cos(glm::radians(m_fAngleY)) * cos(glm::radians(m_fAngleX));
        front.y = sin(glm::radians(m_fAngleX));
        front.z = sin(glm::radians(m_fAngleY)) * cos(glm::radians(m_fAngleX));
        front = glm::normalize(front);

        
        return glm::lookAt(m_Position, m_Position + front, glm::vec3(0.0f, 1.0f, 0.0f));
    }
};