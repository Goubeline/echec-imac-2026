#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class ShaderProgram {
    GLuint m_id;
public:
    ShaderProgram() : m_id(0) {}
    void load(const std::string& vs, const std::string& fs) {
        auto compile = [](GLenum type, const std::string& path) {
            std::ifstream f(path);
            std::stringstream s; s << f.rdbuf();
            std::string code = s.str();
            const char* ptr = code.c_str();
            GLuint shader = glCreateShader(type);
            glShaderSource(shader, 1, &ptr, NULL);
            glCompileShader(shader);
            return shader;
        };
        GLuint v = compile(GL_VERTEX_SHADER, vs);
        GLuint f = compile(GL_FRAGMENT_SHADER, fs);
        m_id = glCreateProgram();
        glAttachShader(m_id, v); glAttachShader(m_id, f);
        glLinkProgram(m_id);
        glDeleteShader(v); glDeleteShader(f);
    }
    GLuint getGLId() const { return m_id; }
    void use() const { glUseProgram(m_id); }
};

struct BoardProgram {
    ShaderProgram m_Program;
    GLint uMVPMatrix, uMVMatrix, uNormalMatrix, uLightDir_vs, uLightIntensityDir, uLightPos_vs, uLightIntensityPoint;
    GLint uKd, uKs, uShininess, uTexture, uUseTexture, uUVScale;

    BoardProgram() {
        m_Program.load("../../shaders/3D.vs.glsl", "../../shaders/3D.fs.glsl");
        GLuint pid = m_Program.getGLId();
        uMVPMatrix = glGetUniformLocation(pid, "uMVPMatrix");
        uMVMatrix = glGetUniformLocation(pid, "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(pid, "uNormalMatrix");
        uKd = glGetUniformLocation(pid, "uKd");
        uKs = glGetUniformLocation(pid, "uKs");
        uShininess = glGetUniformLocation(pid, "uShininess");
        uLightDir_vs = glGetUniformLocation(pid, "uLightDir_vs");
        uLightIntensityDir = glGetUniformLocation(pid, "uLightIntensityDir");
        uLightPos_vs = glGetUniformLocation(pid, "uLightPos_vs");
        uLightIntensityPoint = glGetUniformLocation(pid, "uLightIntensityPoint");
        uTexture = glGetUniformLocation(pid, "uTexture");
        uUseTexture = glGetUniformLocation(pid, "uUseTexture");
        uUVScale = glGetUniformLocation(pid, "uUVScale");
    }
    void use() { m_Program.use(); }
};

struct SkyboxProgram {
    ShaderProgram m_Program;
    GLint uMVP, uTexture, uDayNightFactor;
    SkyboxProgram() {
        m_Program.load("../../shaders/skybox.vs.glsl", "../../shaders/skybox.fs.glsl");
        uMVP = glGetUniformLocation(m_Program.getGLId(), "uMVP");
        uTexture = glGetUniformLocation(m_Program.getGLId(), "uSkyboxTexture");
        uDayNightFactor = glGetUniformLocation(m_Program.getGLId(), "uDayNightFactor");
    }
    void use() { m_Program.use(); }
};