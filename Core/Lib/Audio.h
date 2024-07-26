// Author: Jake Rieger
// Created: 7/25/2024.
//

#pragma once

#include "Platform.h"

#include <string>
#include <vector>

namespace Atom::Audio {
    struct WAVFile {
        WAVEFORMATEX Format;
        std::vector<BYTE> Data;
    };

    IXAudio2* GetXAudio2();
    IXAudio2MasteringVoice* GetMasteringVoice();

    bool Initialize();
    void Shutdown();
    void PlayOneShot(const std::string& path);
    void PlayLoop(const std::string& path);
}  // namespace Atom::Audio