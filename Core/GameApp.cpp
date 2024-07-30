// Author: Jake Rieger
// Created: 7/26/2024.
//

#include "GameApp.h"
#include "Context.h"

#include <utility>

namespace Atom {
    IGameApp::IGameApp(str name) : m_Name(std::move(name)) {
        m_Window = std::make_unique<GameWindow>(1280, 720);
        m_Window->Initialize(m_Name);
        m_ActiveScene = new Scene();

        GraphicsContext::Initialize(m_Window->GetHandle());
    }

    int IGameApp::Run() {
        m_Timer.Start();
        m_ActiveScene->Start();

        while (!m_Window->ShouldClose()) {
            // Frame timer
            auto start = GetTimestamp();

            // IMPORTANT: MAKE SURE WE DISPATCH WINDOWS API MESSAGES OR THE WINDOW IS BRICKED
            m_Window->DispatchMessages();

            // Do game loop stuff here!
            {
                m_ActiveScene->Update(m_Timer.GetDeltaTime());

                // Render
                {
                    // Waits for the previous frame to finish
                    GraphicsContext::RequestNewFrame();
                    // Resets command list and updates render target views
                    GraphicsContext::BeginFrame();

                    // Drawing commands go here

                    // Presents the frame and swaps buffers
                    GraphicsContext::EndFrame();
                }

                m_ActiveScene->LateUpdate();
            }

            auto end             = GetTimestamp();
            Milliseconds elapsed = end - start;
            const auto fT        = 1000.f / elapsed.count();
            const auto fmt       = std::format("{} | FPS: {:.2f} <DX12>", m_Name, fT);
            ::SetWindowTextA(m_Window->GetHandle(), fmt.c_str());
        }

        GraphicsContext::Shutdown();
        m_ActiveScene->Destroyed();

        return 0;
    }

    IGameApp::~IGameApp() {
        m_Window->Shutdown();
        m_Window.reset();

        delete m_ActiveScene;
        m_ActiveScene = nullptr;
    }
}  // namespace Atom
