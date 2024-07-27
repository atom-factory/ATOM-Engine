// Author: Jake Rieger
// Created: 7/25/2024.
//

#include "Audio.h"

using namespace Atom;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    Audio::Initialize();

    Audio::PlayOneShot("");

    for (;;) {
    }

    Audio::Shutdown();
    return 0;
}
