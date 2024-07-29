// Author: Jake Rieger
// Created: 7/28/2024.
//

#include "Shader.h"
#include "Platform.h"

namespace Atom {
    Shader* Shader::Create(const char* vertexSource, const char* fragmentSource) {
        const GLuint vertexShader   = CompileShader(GL_VERTEX_SHADER, vertexSource);
        const GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

        const GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        GLint success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (success == GL_FALSE) {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            MessageBox(nullptr, infoLog, "Shader Linking Error", MB_OK | MB_ICONEXCLAMATION);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return new Shader(shaderProgram);
    }

    void Shader::SetBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(m_Program, name.c_str()), static_cast<int>(value));
    }

    void Shader::SetInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(m_Program, name.c_str()), value);
    }

    void Shader::SetFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(m_Program, name.c_str()), value);
    }

    void Shader::SetVec2(const std::string& name, const glm::vec2& value) const {
        glUniform2fv(glGetUniformLocation(m_Program, name.c_str()), 1, &value[0]);
    }

    void Shader::SetVec2(const std::string& name, float x, float y) const {
        glUniform2f(glGetUniformLocation(m_Program, name.c_str()), x, y);
    }

    void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(m_Program, name.c_str()), 1, &value[0]);
    }

    void Shader::SetVec3(const std::string& name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(m_Program, name.c_str()), x, y, z);
    }

    void Shader::SetVec4(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(m_Program, name.c_str()), 1, &value[0]);
    }

    void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(m_Program, name.c_str()), x, y, z, w);
    }

    void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const {
        glUniformMatrix2fv(glGetUniformLocation(m_Program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const {
        glUniformMatrix3fv(glGetUniformLocation(m_Program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(m_Program, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    GLuint Shader::CompileShader(GLenum type, const char* source) {
        const GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            MessageBox(nullptr, infoLog, "Shader Compilation Error", MB_OK | MB_ICONEXCLAMATION);
        }

        return shader;
    }
}  // namespace Atom