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

void ExampleApp::Initialize() {
    // This is where you can create your scenes, add game objects, and load any data needed
    // before gameplay actually starts.
}

void ExampleApp::Shutdown() {
    // This is where you clean up everything you created on initialization. Anything that isn't
    // a member of IGameApp needs to be manually cleaned up.
}

PLATFORM_MAIN {
    ExampleApp app;
    return app.Run();
}