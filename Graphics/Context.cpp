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

    Vector2 GetScreenSize();

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

        // Create the command queue
        D3D12_COMMAND_QUEUE_DESC qd = {};
        qd.Flags                    = D3D12_COMMAND_QUEUE_FLAG_NONE;
        qd.Type                     = D3D12_COMMAND_LIST_TYPE_DIRECT;
        ThrowIfFailed(g_Device->CreateCommandQueue(&qd, IID_PPV_ARGS(&g_CommandQueue)));

        // Create the swap chain
        DXGI_SWAP_CHAIN_DESC1 scd = {};
        scd.BufferCount           = 2;
        scd.Width                 = CAST<u32>(GetScreenSize().X);
        scd.Height                = CAST<u32>(GetScreenSize().Y);
        scd.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        scd.SampleDesc.Count      = 1;
        scd.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;

        ComPtr<IDXGISwapChain1> swapChain;
        ThrowIfFailed(factory->CreateSwapChainForHwnd(g_CommandQueue.Get(),
                                                      hwnd,
                                                      &scd,
                                                      nullptr,
                                                      nullptr,
                                                      &swapChain));
        ThrowIfFailed(swapChain.As(&g_SwapChain));
        g_FrameIndex = g_SwapChain->GetCurrentBackBufferIndex();

        // Create the render target views

        // Create the command allocator and list

        // Close the command list since we will start in the recording state

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