// Author: Jake Rieger
// Created: 7/25/2024.
//

#include "Audio.h"

using namespace Atom;

int main() {
    Audio::Initialize();

    Audio::PlayOneShot("");

    for (;;) {
    }

    Audio::Shutdown();
    return 0;
}
