#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class TrackballCamera {
private:
    float m_fDistance; // ->par rapport au centre du plateau
    float m_fAngleX;   
    float m_fAngleY;   
    glm::vec3 m_Target; // point qu'on regarde (centre)

public:
    TrackballCamera() {
        m_fDistance = 10.0f; // recule un peu
        m_fAngleX = 45.0f;   // regarde vers le bas
        m_fAngleY = 45.0f;   // de côté
        m_Target = glm::vec3(3.5f, 0.0f, 3.5f); //centre
    }

    // Tourner autour de l'axe Y (gauche/droite)
    void rotateLeft(float degrees) { 
        m_fAngleY += degrees; 
    }

    // Tourner autour de l'axe X (haut/bas)
    void rotateUp(float degrees) {
        m_fAngleX += degrees;
        // Pour ne pas passer sous l'échiquier :
        if(m_fAngleX < 5.0f) m_fAngleX = 5.0f;   // Bloqué (presque) au ras du sol
        if(m_fAngleX > 85.0f) m_fAngleX = 85.0f; // "" à la verticale au dessus
    }

    
    void zoom(float factor) {
        m_fDistance -= factor;
        if(m_fDistance < 2.0f) m_fDistance = 2.0f; // Ne pas rentrer dans le plateau
        if(m_fDistance > 20.0f) m_fDistance = 20.0f; // Ne pas partir trop loin
    }

    //  matrice de vue (Comme TD)
    glm::mat4 getViewMatrix() const {
        glm::mat4 view = glm::mat4(1.0f);
        // 1. On recule la caméra
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -m_fDistance));
        // 2. On applique les rotations
        view = glm::rotate(view, glm::radians(m_fAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::rotate(view, glm::radians(m_fAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
        // 3. On se centre sur la cible
        view = glm::translate(view, -m_Target);
        
        return view;
    }
};