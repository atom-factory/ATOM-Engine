// Author: Jake Rieger
// Created: 7/28/2024.
//

#include "Context.h"
#include "Shader.h"
#include "MathUtils.h"
#include "Core/Camera.h"

#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/gl.h>

namespace Atom {
    static HWND g_TargetHWND;

    bool GraphicsContext::Initialize(HWND hwnd) {
        if (!hwnd)
            return false;

        g_TargetHWND = hwnd;

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

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return true;
    }

    void GraphicsContext::BeginFrame() {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GraphicsContext::EndFrame() {
        const auto hdc = ::GetDC(g_TargetHWND);
        ::SwapBuffers(hdc);
        ::ReleaseDC(g_TargetHWND, hdc);
    }

    void GraphicsContext::Resize(const u32 width, const u32 height) {
        glViewport(0, 0, CAST<i32>(width), CAST<i32>(height));
    }

    Vector2 GetScreenSize() {
        RECT rc;
        ::GetClientRect(g_TargetHWND, &rc);
        return Vector2(rc.right - rc.left, rc.bottom - rc.top);
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
                     CAST<GLsizeiptr>(vertices.size()) * sizeof(f32),
                     vertices.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     CAST<GLsizeiptr>(indices.size()) * sizeof(unsigned int),
                     indices.data(),
                     GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(f32), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        return VAO;
    }

    void GenerateRectVertices(const f32 centerX,
                              const f32 centerY,
                              const f32 width,
                              const f32 height,
                              std::vector<f32>& vertices,
                              std::vector<unsigned int>& indices) {
        const f32 halfWidth  = width / 2.0f;
        const f32 halfHeight = height / 2.0f;

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

    void GenerateEllipseVertices(const f32 centerX,
                                 const f32 centerY,
                                 const f32 radiusX,
                                 const f32 radiusY,
                                 const i32 segments,
                                 std::vector<f32>& vertices,
                                 std::vector<u32>& indices) {
        vertices.clear();
        const f32 angleIncrement = 2.0f * Math::kPI / CAST<f32>(segments);
        for (int i = 0; i <= segments; ++i) {
            const f32 angle = CAST<f32>(i) * angleIncrement;
            f32 x           = centerX + radiusX * std::cos(angle);
            f32 y           = centerY + radiusY * std::sin(angle);
            vertices.push_back(x);
            vertices.push_back(y);
        }

        indices.clear();
        for (int i = 0; i < segments; ++i) {
            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
        }
    }

    void GraphicsContext::DrawRectangle(const Vector2 size,
                                        const Vector2 position,
                                        const Color& fillColor) {
        const auto shader         = Shader::Create(kDefaultVertexShader, kDefaultFragmentShader);
        std::vector<f32> vertices = {};
        std::vector<u32> indices  = {};
        GenerateRectVertices(position.X, position.Y, size.X, size.Y, vertices, indices);
        const auto vao = CreateVAO(vertices, indices);

        glUseProgram(shader->GetProgram());
        shader->SetMat4("uProjection", Camera::GetProjectionMatrix(GetScreenSize()));
        shader->SetVec4("uColor",
                        {
                          fillColor.Red,
                          fillColor.Green,
                          fillColor.Blue,
                          fillColor.Alpha,
                        });
        glBindVertexArray(vao);

        // draw rectangle
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        delete shader;
    }

    void GraphicsContext::DrawEllipse(
      f32 centerX, f32 centerY, f32 radiusX, f32 radiusY, int segments, const Color& fillColor) {
        const auto shader         = Shader::Create(kDefaultVertexShader, kDefaultFragmentShader);
        std::vector<f32> vertices = {};
        std::vector<u32> indices  = {};
        GenerateEllipseVertices(centerX, centerY, radiusX, radiusY, segments, vertices, indices);
        const auto vao = CreateVAO(vertices, indices);

        glUseProgram(shader->GetProgram());
        shader->SetMat4("uProjection", Camera::GetProjectionMatrix(GetScreenSize()));
        shader->SetVec4("uColor",
                        {
                          fillColor.Red,
                          fillColor.Green,
                          fillColor.Blue,
                          fillColor.Alpha,
                        });

        glBindVertexArray(vao);

        glDrawElements(GL_TRIANGLES, segments * 3, GL_UNSIGNED_INT, nullptr);

        delete shader;
    }
}  // namespace Atom