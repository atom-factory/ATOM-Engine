// Author: Jake Rieger
// Created: 7/27/2024.
//

#include "Platform.h"
#include "GameApp.h"

class ExampleApp final : public Atom::IGameApp {
public:
    ExampleApp() {}
    void Initialize() override;
    void Shutdown() override;
};

void ExampleApp::Initialize() {}

void ExampleApp::Shutdown() {}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    const ExampleApp app;

    app.Run();

    return 0;
}