// Author: Jake Rieger
// Created: 7/27/2024.
//

#pragma once

#include "Platform.h"
#include "Types.h"

namespace Atom {
    class GameWindow {
    public:
        GameWindow(const UINT width, const UINT height) : m_Handle(nullptr), m_Instance(nullptr) {
            m_Size.X      = CAST<f32>(width);
            m_Size.Y      = CAST<f32>(height);
            m_ShouldClose = false;
        }
        void Initialize();
        void Shutdown();

        HWND GetHandle() const;
        Vector2 GetSize() const;

        void DispatchMessages();
        bool ShouldClose() const;

    private:
        static LRESULT CALLBACK MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT OnResize(UINT width, UINT height);
        LRESULT OnKeyUp();
        LRESULT OnKeyDown();
        LRESULT OnLeftMouseButtonDown();
        LRESULT OnLeftMouseButtonUp();
        LRESULT OnRightMouseButtonDown();
        LRESULT OnRightMouseButtonUp();
        LRESULT OnMouseMove(UINT xPos, UINT yPos);

        Vector2 m_Size;
        HWND m_Handle;
        HINSTANCE m_Instance;
        MSG m_Message = {};
        bool m_ShouldClose;
    };
}  // namespace Atom