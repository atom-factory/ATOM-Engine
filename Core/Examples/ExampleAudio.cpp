// Author: Jake Rieger
// Created: 7/25/2024.
//

#include "Audio.h"

using namespace Atom;

int main() {
    Audio::Initialize();

    Audio::PlayOneShot("C:/Users/conta/Desktop/Songs/ABYSS WATCHER.wav");

    Audio::Shutdown();
    return 0;
}