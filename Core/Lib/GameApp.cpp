// Author: Jake Rieger
// Created: 7/26/2024.
//

#include "GameApp.h"

namespace Atom {
    IGameApp::IGameApp() {
        m_Window = std::make_unique<GameWindow>(800, 600);
        m_Window->Initialize();
    }

    void IGameApp::Run() const {
        while (!m_Window->ShouldClose()) {
            // IMPORTANT: MAKE SURE WE DISPATCH WINDOWS API MESSAGES OR THE WINDOW IS BRICKED
            m_Window->DispatchMessages();

            // Do game loop stuff here!
        }
    }

    IGameApp::~IGameApp() {
        m_Window->Shutdown();
        m_Window.reset();
    }
}  // namespace Atom
