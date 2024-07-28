// Author: Jake Rieger
// Created: 7/28/2024.
//

#pragma once

#include <GL/glew.h>

namespace Atom {
    static auto kDefaultVertexShader   = R""(
        #version 330 core
        layout(location = 0) in vec2 position;
        void main() {
            gl_Position = vec4(position, 0.0, 1.0);
        }
    )"";
    static auto kDefaultFragmentShader = R""(
    #version 330 core
        out vec4 FragColor;
        void main() {
            FragColor = vec4(1.0, 0.0, 0.0, 1.0);
        }
    )"";

    class Shader {
    public:
        static GLuint Create(const char* vertexSource, const char* fragmentSource);

    private:
        static GLuint CompileShader(GLenum type, const char* source);
    };
}  // namespace Atom
