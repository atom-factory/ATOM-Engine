// Author: Jake Rieger
// Created: 7/28/2024.
//

#include "Context.h"

#include <DirectXTK12/Inc/GraphicsMemory.h>
#include <DirectXTK12/Src/PlatformHelpers.h>

namespace Atom {
    static HWND g_TargetHWND;
    static ComPtr<ID3D12Device> g_Device;
    static ComPtr<IDXGISwapChain3> g_SwapChain;
    static ComPtr<ID3D12CommandQueue> g_CommandQueue;
    static ComPtr<ID3D12DescriptorHeap> g_RTVHeap;
    static ComPtr<ID3D12Resource> g_RenderTargets[2];
    static ComPtr<ID3D12GraphicsCommandList> g_CommandList;
    static ComPtr<ID3D12CommandAllocator> g_CommandAllocator;
    static u32 g_RTVDescriptorSize;
    static u32 g_FrameIndex;

    bool GraphicsContext::Initialize(HWND hwnd) {
        if (!hwnd)
            return false;

        g_TargetHWND = hwnd;

        u32 dxgiFactoryFlags = 0;
#ifndef NDEBUG
        ComPtr<ID3D12Debug> debugController;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
            debugController->EnableDebugLayer();
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
#endif

        // Create DXGI factory
        ComPtr<IDXGIFactory7> factory;
        ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

        // Create DX12 device
        ComPtr<IDXGIAdapter4> adapter;
        for (u32 adapterIndex = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapterByGpuPreference(
                                                             adapterIndex,
                                                             DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                                                             IID_PPV_ARGS(&adapter));
             ++adapterIndex) {
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(),
                                            D3D_FEATURE_LEVEL_12_1,
                                            _uuidof(ID3D12Device),
                                            nullptr))) {
                break;
            }
        }

        ThrowIfFailed(
          D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&g_Device)));

        return true;
    }

    void GraphicsContext::BeginFrame() {}

    void GraphicsContext::EndFrame() {}

    void GraphicsContext::Resize(const u32 width, const u32 height) {}

    Vector2 GetScreenSize() {
        RECT rc;
        ::GetClientRect(g_TargetHWND, &rc);
        return Vector2(CAST<f32>(rc.right - rc.left), CAST<f32>(rc.bottom - rc.top));
    }
}  // namespace Atom