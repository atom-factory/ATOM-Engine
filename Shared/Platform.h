// Author: Jake Rieger
// Created: 7/25/2024.
//

#pragma once
#pragma warning(disable : 4996)

#include <codecvt>
#include <cstdio>
#include <locale>
#include <stdexcept>

#include <Windows.h>
#include <windowsx.h>
#include <wrl/client.h>

#include <xaudio2.h>
#include <dxgi1_6.h>
#include <d3d12.h>
// #include <DirectXMath.h>
// #include <DirectXColors.h>
#include <d3dx12.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "xaudio2.lib")

// using namespace DirectX;
using Microsoft::WRL::ComPtr;

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