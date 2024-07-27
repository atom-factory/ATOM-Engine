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
            m_Size.X = width;
            m_Size.Y = height;
        }
        void Initialize();
        void Run() const;
        void Shutdown();

        HWND GetHandle() const;
        Vector2 GetSize() const;

    private:
        void MainLoop() const;
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
    };
}  // namespace Atom