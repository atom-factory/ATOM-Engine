// Author: Jake Rieger
// Created: 7/28/2024.
//

#include "Shader.h"
#include "Platform.h"

namespace Atom {
    GLuint Shader::Create(const char* vertexSource, const char* fragmentSource) {
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

        return shaderProgram;
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