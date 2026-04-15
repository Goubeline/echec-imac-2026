#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// !! RETROUVER loadProgramm() pour corriger, pour l'instant LoadShader
inline GLuint LoadShader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile(vertexPath), fShaderFile(fragmentPath);
    if(vShaderFile.is_open() && fShaderFile.is_open()) {
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf(); fShaderStream << fShaderFile.rdbuf();
        vertexCode = vShaderStream.str(); fragmentCode = fShaderStream.str();
    } else { std::cout << "ERREUR SHADER NON TROUVE" << std::endl; }
    
    const char* vShaderCode = vertexCode.c_str(); const char* fShaderCode = fragmentCode.c_str();
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER), fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL); glCompileShader(vertex);
    glShaderSource(fragment, 1, &fShaderCode, NULL); glCompileShader(fragment);
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex); glAttachShader(program, fragment); glLinkProgram(program);
    return program;
}


struct BoardProgram {
    GLuint m_Program;

   
    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uColor; 

    BoardProgram() {
     // Le "../../" permet de remonter depuis bin/Debug/ vers la racine du projet sinon plante 
m_Program = LoadShader("../../shaders/3D.vs.glsl", "../../shaders/3D.fs.glsl");
        
        // On récupère les adresses (comme constructeur)
        uMVPMatrix = glGetUniformLocation(m_Program, "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(m_Program, "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program, "uNormalMatrix");
        uColor = glGetUniformLocation(m_Program, "uColor");
    }
    
    void use() {
        glUseProgram(m_Program);
    }
};