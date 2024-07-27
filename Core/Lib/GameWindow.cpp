// Author: Jake Rieger
// Created: 7/27/2024.
//

#include "GameWindow.h"

namespace Atom {
    void GameWindow::Initialize() {
        m_Instance = ::GetModuleHandle(nullptr);

        WNDCLASSEXA wc;
        wc.style         = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc   = GameWindow::MessageHandler;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = m_Instance;
        wc.hIcon         = ::LoadIcon(nullptr, IDI_APPLICATION);
        wc.hIconSm       = wc.hIcon;
        wc.hCursor       = ::LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wc.lpszMenuName  = nullptr;
        wc.lpszClassName = "AtomGameWindowClass";
        wc.cbSize        = sizeof(WNDCLASSEXW);
        wc.cbWndExtra    = sizeof(GameWindow);

        if (const auto registerResult = ::RegisterClassExA(&wc); registerResult <= 0) {
            m_ShouldClose = true;
        }

        const u32 scrWidth  = ::GetSystemMetrics(SM_CXSCREEN);
        const u32 scrHeight = ::GetSystemMetrics(SM_CYSCREEN);
        const u32 posX      = CAST<u32>((scrWidth - m_Size.X) / 2);
        const u32 posY      = CAST<u32>((scrHeight - m_Size.Y) / 2);

        m_Handle = ::CreateWindowExA(0,
                                     "AtomGameWindowClass",
                                     "ATOM Game Window",
                                     WS_OVERLAPPEDWINDOW,
                                     CAST<i32>(posX),
                                     CAST<i32>(posY),
                                     CAST<i32>(m_Size.X),
                                     CAST<i32>(m_Size.Y),
                                     nullptr,
                                     nullptr,
                                     m_Instance,
                                     this);
        if (!m_Handle) {
            m_ShouldClose = true;
        }

        ::SetWindowLongPtr(m_Handle, GWLP_USERDATA, RCAST<LONG_PTR>(this));

        ::ShowWindow(m_Handle, SW_SHOW);
        ::UpdateWindow(m_Handle);
    }

    void GameWindow::Shutdown() {
        if (m_Handle) {
            ::DestroyWindow(m_Handle);
            m_Handle = nullptr;
        }
    }

    HWND GameWindow::GetHandle() const {
        return m_Handle;
    }

    Vector2 GameWindow::GetSize() const {
        return m_Size;
    }

    bool GameWindow::ShouldClose() const {
        return m_ShouldClose;
    }

    void GameWindow::DispatchMessages() {
        if (::PeekMessage(&m_Message, nullptr, 0, 0, PM_REMOVE)) {
            ::TranslateMessage(&m_Message);
            ::DispatchMessage(&m_Message);
        }
    }

    LRESULT GameWindow::MessageHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        const LONG_PTR userData = ::GetWindowLongPtr(hwnd, GWLP_USERDATA);
        const auto gameWindow   = RCAST<GameWindow*>(userData);

        if (!gameWindow) {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }

        switch (msg) {
            case WM_DESTROY:
                ::PostQuitMessage(0);
                return 0;
            case WM_CLOSE:
                gameWindow->m_ShouldClose = true;
                ::PostQuitMessage(0);
                return 0;
            case WM_SIZE: {
                const UINT w = LOWORD(lParam);
                const UINT h = HIWORD(lParam);
                return gameWindow->OnResize(w, h);
            }
            case WM_PAINT:
                ::ValidateRect(hwnd, nullptr);
                return 0;
            case WM_DISPLAYCHANGE:
                ::InvalidateRect(hwnd, nullptr, false);
                return 0;
            case WM_KEYDOWN:
                return gameWindow->OnKeyDown();
            case WM_KEYUP:
                return gameWindow->OnKeyUp();
            case WM_LBUTTONDOWN:
                return gameWindow->OnLeftMouseButtonDown();
            case WM_LBUTTONUP:
                return gameWindow->OnLeftMouseButtonUp();
            case WM_RBUTTONDOWN:
                return gameWindow->OnRightMouseButtonDown();
            case WM_RBUTTONUP:
                return gameWindow->OnRightMouseButtonUp();
            case WM_MOUSEMOVE: {
                const auto xPos = GET_X_LPARAM(lParam);
                const auto yPos = GET_Y_LPARAM(lParam);
                return gameWindow->OnMouseMove(xPos, yPos);
            }
            default:
                break;
        }

        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    LRESULT GameWindow::OnResize(UINT width, UINT height) {
        m_Size.X = CAST<f32>(width);
        m_Size.Y = CAST<f32>(height);
        // Resize render context
        return S_OK;
    }

    LRESULT GameWindow::OnKeyUp() {
        return S_OK;
    }

    LRESULT GameWindow::OnKeyDown() {
        return S_OK;
    }

    LRESULT GameWindow::OnLeftMouseButtonDown() {
        return S_OK;
    }

    LRESULT GameWindow::OnLeftMouseButtonUp() {
        return S_OK;
    }

    LRESULT GameWindow::OnRightMouseButtonDown() {
        return S_OK;
    }

    LRESULT GameWindow::OnRightMouseButtonUp() {
        return S_OK;
    }

    LRESULT GameWindow::OnMouseMove(UINT xPos, UINT yPos) {
        return S_OK;
    }
}  // namespace Atom