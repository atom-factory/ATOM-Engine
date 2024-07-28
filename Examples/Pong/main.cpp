// Author: Jake Rieger
// Created: 7/28/2024.
//

#include "Platform.h"
#include "GameApp.h"

class Pong final : public Atom::IGameApp {
public:
    Pong() : IGameApp("Pong") {};
    void Initialize() override {}
    void Shutdown() override {}
};

WIN32_MAIN {
    Pong app;
    return app.Run();
}