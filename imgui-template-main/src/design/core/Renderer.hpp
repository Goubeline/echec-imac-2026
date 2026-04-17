#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp> // Pour glm::pi
#include <memory>
#include "Programs.hpp"

// classe qui contient tout ce qui est commun aux rendus 3D
class Renderer {
protected:
    std::unique_ptr<BoardProgram> m_program;

    Renderer() {
        m_program = std::make_unique<BoardProgram>();
    }

    virtual ~Renderer() = default;

    void sendMatrices(const glm::mat4& mvp, const glm::mat4& mv) {
        glUniformMatrix4fv(m_program->uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvp));
        glUniformMatrix4fv(m_program->uMVMatrix, 1, GL_FALSE, glm::value_ptr(mv));
        glUniformMatrix4fv(m_program->uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(mv))));
    }
  
void applyLighting(const glm::mat4& view, float dayNightFactor, float ks, float shininess) {
        m_program->use();
        
        glUniform3f(m_program->uKs, ks, ks, ks); 
        glUniform1f(m_program->uShininess, shininess);

        float smoothFactor = dayNightFactor * dayNightFactor * (3.0f - 2.0f * dayNightFactor);

        // =====================================================================
        // 1.DIFFUSE
        // =====================================================================
        float dirIntensity;
        if (smoothFactor < 0.5f) dirIntensity = glm::mix(0.85f, 0.55f, smoothFactor * 2.0f);
        else dirIntensity = glm::mix(0.55f, 0.25f, (smoothFactor - 0.5f) * 2.0f);

        
        glm::vec3 dirWorld = glm::normalize(glm::vec3(0.5f, 0.6f, 0.4f)); 
        glm::vec3 dirView = glm::vec3(view * glm::vec4(dirWorld, 0.0f)); 
        glUniform3fv(m_program->uLightDir_vs, 1, glm::value_ptr(dirView));
        
        glm::vec3 dirColor = glm::mix(glm::vec3(1.0f, 0.95f, 0.9f), glm::vec3(0.9f, 0.7f, 0.5f), smoothFactor);
        glUniform3fv(m_program->uLightIntensityDir, 1, glm::value_ptr(dirColor * dirIntensity)); 

        // =====================================================================
        // 2. SOURCE 2 : LUM MOBILE 
        // =====================================================================
        float z = glm::mix(-6.0f, 13.0f, smoothFactor);
        
        float y = 5.0f + sin(smoothFactor * glm::pi<float>()) * 3.0f; 
        
        
        glm::vec3 posWorld(-5.0f, y, z); 
        
        glm::vec3 posView = glm::vec3(view * glm::vec4(posWorld, 1.0f));
        glUniform3fv(m_program->uLightPos_vs, 1, glm::value_ptr(posView));

       
        glm::vec3 colorA(1.0f, 0.9f, 0.8f);    
        float intA = 25.0f;                   
        glm::vec3 colorB(1.0f, 0.5f, 0.2f);    
        float intB = 30.0f;                   
        glm::vec3 colorC(0.3f, 0.5f, 1.0f);    
        float intC = 25.0f;                   

        glm::vec3 magicColor;
        float currentIntensity;
        if (smoothFactor < 0.5f) {
            float t = smoothFactor * 2.0f;
            magicColor = glm::mix(colorA, colorB, t);
            currentIntensity = glm::mix(intA, intB, t);
        } else {
            float t = (smoothFactor - 0.5f) * 2.0f;
            magicColor = glm::mix(colorB, colorC, t);
            currentIntensity = glm::mix(intB, intC, t);
        }

        glm::vec3 finalIntensity = magicColor * currentIntensity;
        glUniform3fv(m_program->uLightIntensityPoint, 1, glm::value_ptr(finalIntensity));
    }
}; 