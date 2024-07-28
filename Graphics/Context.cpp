// Author: Jake Rieger
// Created: 7/28/2024.
//

#include "Context.h"

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/gl.h>

namespace Atom {
    bool GraphicsContext::Initialize(HWND hwnd) {
        if (!hwnd)
            return false;

        const auto hdc = ::GetDC(hwnd);

        PIXELFORMATDESCRIPTOR pfd;
        pfd.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion     = 1;
        pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType   = PFD_TYPE_RGBA;
        pfd.cColorBits   = 32;
        pfd.cDepthBits   = 24;
        pfd.cStencilBits = 8;
        pfd.iLayerType   = PFD_MAIN_PLANE;

        int format = ::ChoosePixelFormat(hdc, &pfd);
        ::SetPixelFormat(hdc, format, &pfd);

        HGLRC tempContext = wglCreateContext(hdc);
        wglMakeCurrent(hdc, tempContext);

        glewExperimental = GL_TRUE;  // TODO: ???
        if (glewInit() != GLEW_OK)
            return false;

        // Enable modern OpenGL context creation
        int attribs[] = {
          WGL_CONTEXT_MAJOR_VERSION_ARB,
          4,
          WGL_CONTEXT_MINOR_VERSION_ARB,
          6,
          WGL_CONTEXT_FLAGS_ARB,
          WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
          0,
        };

        if (wglewIsSupported("WGL_ARB_create_context") == 1) {
            HGLRC hrc = wglCreateContextAttribsARB(hdc, nullptr, attribs);
            wglMakeCurrent(nullptr, nullptr);
            wglDeleteContext(tempContext);
            wglMakeCurrent(hdc, hrc);
        } else {
            // Unable to create modern OpenGL context, using old one
            wglMakeCurrent(hdc, tempContext);
        }

        ::ReleaseDC(hwnd, hdc);

        RECT rc;
        ::GetClientRect(hwnd, &rc);
        glViewport(0, 0, rc.right, rc.bottom);

        return true;
    }

    void GraphicsContext::BeginFrame() {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GraphicsContext::EndFrame(HWND hwnd) {
        const auto hdc = ::GetDC(hwnd);
        ::SwapBuffers(hdc);
        ::ReleaseDC(hwnd, hdc);
    }

    void GraphicsContext::Resize(const u32 width, const u32 height) {
        glViewport(0, 0, CAST<i32>(width), CAST<i32>(height));
    }

    u32 GraphicsContext::CreateVAO(const std::vector<f32>& vertices,
                                   const std::vector<u32>& indices) {
        GLuint VAO, VBO, EBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     CAST<GLsizeiptr>(vertices.size()) * sizeof(float),
                     vertices.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     CAST<GLsizeiptr>(indices.size()) * sizeof(unsigned int),
                     indices.data(),
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return VAO;
    }

    void GenerateRectVertices(const float centerX,
                              const float centerY,
                              const float width,
                              const float height,
                              std::vector<float>& vertices,
                              std::vector<unsigned int>& indices) {
        const float halfWidth  = width / 2.0f;
        const float halfHeight = height / 2.0f;

        vertices = {
          centerX - halfWidth,
          centerY - halfHeight,  // Bottom-left
          centerX + halfWidth,
          centerY - halfHeight,  // Bottom-right
          centerX + halfWidth,
          centerY + halfHeight,  // Top-right
          centerX - halfWidth,
          centerY + halfHeight  // Top-left
        };

        indices = {0, 1, 2, 2, 3, 0};
    }

    void GraphicsContext::DrawRectangle(const Vector2 size,
                                        const Vector2 position,
                                        Color color,
                                        const u32 shader) {
        std::vector<float> vertices = {};
        std::vector<u32> indices    = {};
        GenerateRectVertices(position.X, position.Y, size.X, size.Y, vertices, indices);
        const auto vao = CreateVAO(vertices, indices);

        glUseProgram(shader);
        glBindVertexArray(vao);

        // draw rectangle
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}  // namespace Atom