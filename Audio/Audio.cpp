// Author: Jake Rieger
// Created: 7/25/2024.
//

#include "Audio.h"

#include <fstream>
#include <iostream>
#include <ostream>
#include <thread>

namespace Atom::Audio {
    IXAudio2 *g_XAudio2;
    IXAudio2MasteringVoice *g_MasterVoice;

    IXAudio2 *GetXAudio2() {
        return g_XAudio2;
    }

    IXAudio2MasteringVoice *GetMasteringVoice() {
        return g_MasterVoice;
    }

    bool Initialize() {
        auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (FAILED(hr)) {
            return false;
        }

        hr = XAudio2Create(&g_XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
        if (FAILED(hr)) {
            return false;
        }

        hr = g_XAudio2->CreateMasteringVoice(&g_MasterVoice);
        if (FAILED(hr)) {
            return false;
        }

        return true;
    }

    void Shutdown() {
        g_MasterVoice->DestroyVoice();
        g_XAudio2->Release();
        CoUninitialize();
    }

    static bool LoadWAVFile(const char *filename, WAVFile &wavFile) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return false;
        }

        // Read header
        file.seekg(0, std::ios::end);
        size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        BYTE header[44];
        file.read(reinterpret_cast<char *>(header), 44);

        if (file.gcount() != 44) {
            std::cerr << "Failed to read WAV header." << std::endl;
            return false;
        }

        // Parse header (assuming it's a valid WAV file)
        wavFile.Format.wFormatTag = *reinterpret_cast<WORD *>(&header[20]);
        wavFile.Format.nChannels = *reinterpret_cast<WORD *>(&header[22]);
        wavFile.Format.nSamplesPerSec = *reinterpret_cast<DWORD *>(&header[24]);
        wavFile.Format.nAvgBytesPerSec = *reinterpret_cast<DWORD *>(&header[28]);
        wavFile.Format.nBlockAlign = *reinterpret_cast<WORD *>(&header[32]);
        wavFile.Format.wBitsPerSample = *reinterpret_cast<WORD *>(&header[34]);
        wavFile.Format.cbSize = 0;

        size_t dataSize = *reinterpret_cast<DWORD *>(&header[40]);
        wavFile.Data.resize(dataSize);
        file.read(reinterpret_cast<char *>(wavFile.Data.data()), dataSize);

        if (file.gcount() != dataSize) {
            std::cerr << "Failed to read WAV data." << std::endl;
            return false;
        }

        return true;
    }

    void PlayOneShot(const std::string &path) {
        std::thread([&,path] {
            WAVFile wavFile = {};
            if (!LoadWAVFile(path.c_str(), wavFile)) {
                std::cerr << "Failed to load WAV file: " << path << std::endl;
                return;
            }

            IXAudio2SourceVoice *sourceVoice;
            auto hr = g_XAudio2->CreateSourceVoice(&sourceVoice, &wavFile.Format);
            if (FAILED(hr)) {
                std::cerr << "Failed to create audio source." << std::endl;
                return;
            }

            XAUDIO2_BUFFER buffer = {};
            buffer.AudioBytes = wavFile.Data.size();
            buffer.pAudioData = wavFile.Data.data();
            buffer.Flags = XAUDIO2_END_OF_STREAM;

            hr = sourceVoice->SubmitSourceBuffer(&buffer);
            if (FAILED(hr)) {
                std::cerr << "Failed to submit audio source buffer." << std::endl;
                return;
            }

            hr = sourceVoice->Start(0);
            if (FAILED(hr)) {
                std::cerr << "Failed to start audio source." << std::endl;
                return;
            }

            std::cout << "Playing audio source." << std::endl;

            XAUDIO2_VOICE_STATE state;
            do {
                sourceVoice->GetState(&state);
                Sleep(100);
            } while (state.BuffersQueued > 0);

            sourceVoice->DestroyVoice();
        }).detach();
    }

    void PlayLoop(const std::string &path) {
    }
} // namespace Atom::Audio
