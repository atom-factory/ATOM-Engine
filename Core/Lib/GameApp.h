// Author: Jake Rieger
// Created: 7/26/2024.
//

#pragma once

#include "Types.h"
#include "GameWindow.h"
#include "Scene.h"
#include "Timer.h"

namespace Atom {
    class IGameApp {
    public:
        IGameApp();
        virtual ~IGameApp();
        virtual void Initialize() = 0;
        virtual void Shutdown()   = 0;

        int Run();

        GameWindow* GetWindow() const {
            return m_Window.get();
        }

    protected:
        Unique<GameWindow> m_Window = nullptr;
        Timer m_Timer;
        Scene* m_ActiveScene = nullptr;
    };
}  // namespace Atom
