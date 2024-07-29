// Author: Jake Rieger
// Created: 7/28/2024.
//

#pragma once

#include "Types.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace Atom {
    static auto kDefaultVertexShader   = R""(
        #version 330 core
        layout(location = 0) in vec2 position;
        uniform mat4 uProjection;
        void main() {
            gl_Position = uProjection * vec4(position, 0.0, 1.0);
        }
    )"";
    static auto kDefaultFragmentShader = R""(
    #version 330 core
        out vec4 FragColor;
        uniform vec4 uColor;
        void main() {
            FragColor = uColor;
        }
    )"";

    class Shader {
    public:
        static Shader* Create(const char* vertexSource, const char* fragmentSource);

        [[nodiscard]] GLuint GetProgram() const {
            return m_Program;
        }

        //=================//
        // Uniform Setters //
        //=================//
        void SetBool(const str& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetVec2(const std::string& name, const glm::vec2& value) const;
        void SetVec2(const std::string& name, float x, float y) const;
        void SetVec3(const std::string& name, const glm::vec3& value) const;
        void SetVec3(const std::string& name, float x, float y, float z) const;
        void SetVec4(const std::string& name, const glm::vec4& value) const;
        void SetVec4(const std::string& name, float x, float y, float z, float w) const;
        void SetMat2(const std::string& name, const glm::mat2& mat) const;
        void SetMat3(const std::string& name, const glm::mat3& mat) const;
        void SetMat4(const std::string& name, const glm::mat4& mat) const;

    private:
        explicit Shader(const GLuint program) : m_Program(program) {};
        static GLuint CompileShader(GLenum type, const char* source);
        GLuint m_Program = 0;
    };
}  // namespace Atom
