// Author: Jake Rieger
// Created: 7/25/2024.
//

#pragma once

#include <Windows.h>
#include <codecvt>
#include <cstdio>
#include <locale>
#include <stdexcept>
#include <windowsx.h>
#include <xaudio2.h>

#pragma comment(lib, "xaudio2.lib")

#define WIN32_MAIN int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)

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

inline void WideToANSI(const std::wstring& value, std::string& converted) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    converted = converter.to_bytes(value);
}

inline void ANSIToWide(const std::string& value, std::wstring& converted) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    converted = converter.from_bytes(value);
}