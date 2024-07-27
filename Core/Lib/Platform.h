// Author: Jake Rieger
// Created: 7/25/2024.
//

#pragma once

#include <Windows.h>
#include <cstdio>
#include <stdexcept>
#include <windowsx.h>
#include <xaudio2.h>

#pragma comment(lib, "xaudio2.lib")

/**
 * @brief Prints debug message to debugger console when app runs in WIN32 mode
 */
inline void DebugPrint(const char* fmt, ...) {
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    OutputDebugStringA(buffer);
}

template<class Interface>
void SafeRelease(Interface** ppInterface) {
    if (*ppInterface != NULL) {
        (*ppInterface)->Release();
        (*ppInterface) = NULL;
    }
}

/**
 * @brief Throws runtime error with message if HRESULT failed
 *
 * @param hr HRESULT
 * @param msg Error message to display if result failed
 */
inline void CheckResult(const HRESULT hr, const char* msg) {
    if (FAILED(hr)) {
        throw std::runtime_error(msg);
    }
}