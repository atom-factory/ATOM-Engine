// Author: Jake Rieger
// Created: 7/26/2024.
//

#include "GameApp.h"

namespace Atom {
    IGameApp::IGameApp() {
        m_Window = std::make_unique<GameWindow>(800, 600);
        m_Window->Initialize();
        m_ActiveScene = new Scene();
    }

    int IGameApp::Run() {
        m_Timer.Start();
        m_ActiveScene->Start();

        while (!m_Window->ShouldClose()) {
            // IMPORTANT: MAKE SURE WE DISPATCH WINDOWS API MESSAGES OR THE WINDOW IS BRICKED
            m_Window->DispatchMessages();

            m_ActiveScene->Update(m_Timer.GetDeltaTime());

            // Do game loop stuff here!
            m_ActiveScene->LateUpdate();
        }

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
