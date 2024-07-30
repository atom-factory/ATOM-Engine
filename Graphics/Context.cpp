// Author: Jake Rieger
// Created: 7/28/2024.
//

#include "Context.h"

namespace Atom {
    static HWND g_TargetHWND;
    static ComPtr<ID3D12Device> g_Device;
    static ComPtr<IDXGISwapChain3> g_SwapChain;
    static ComPtr<ID3D12CommandQueue> g_CommandQueue;
    static ComPtr<ID3D12DescriptorHeap> g_RTVHeap;
    static ComPtr<ID3D12Resource> g_RenderTargets[2];
    static u32 g_RTVDescriptorSize;
    static u32 g_FrameIndex;

    bool GraphicsContext::Initialize(HWND hwnd) {
        if (!hwnd)
            return false;

        g_TargetHWND = hwnd;

        return true;
    }

    void GraphicsContext::BeginFrame() {}

    void GraphicsContext::EndFrame() {}

    void GraphicsContext::Resize(const u32 width, const u32 height) {}

    Vector2 GetScreenSize() {
        RECT rc;
        ::GetClientRect(g_TargetHWND, &rc);
        return Vector2(rc.right - rc.left, rc.bottom - rc.top);
    }
}  // namespace Atom