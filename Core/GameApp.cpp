// Author: Jake Rieger
// Created: 7/26/2024.
//

#include "GameApp.h"

#include "Graphics/Context.h"
#include "Graphics/Shader.h"

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

        auto shader = Shader::Create(kDefaultVertexShader, kDefaultFragmentShader);

        while (!m_Window->ShouldClose()) {
            // IMPORTANT: MAKE SURE WE DISPATCH WINDOWS API MESSAGES OR THE WINDOW IS BRICKED
            m_Window->DispatchMessages();

            // Do game loop stuff here!
            {
                m_ActiveScene->Update(m_Timer.GetDeltaTime());

                // Render
                GraphicsContext::BeginFrame();
                GraphicsContext::DrawRectangle({0.5, 0.5}, {0, 0}, Colors::Cyan, shader);
                GraphicsContext::EndFrame(m_Window->GetHandle());

                m_ActiveScene->LateUpdate();
            }
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
