// Author: Jake Rieger
// Created: 7/26/2024.
//

#pragma once

#include "Types.h"
#include "GameWindow.h"
#include "Timer.h"

namespace Atom {
    class IGameApp {
    public:
        IGameApp();
        virtual ~IGameApp();
        virtual void Initialize() = 0;
        virtual void Shutdown()   = 0;

        void Run();

        GameWindow* GetWindow() const {
            return m_Window.get();
        }

    private:
        Unique<GameWindow> m_Window = nullptr;
        Timer m_Timer;
    };
}  // namespace Atom
