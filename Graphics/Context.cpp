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

        // Create the descriptor heap and render target views
        D3D12_DESCRIPTOR_HEAP_DESC rtvDesc = {};
        rtvDesc.NumDescriptors             = 2;
        rtvDesc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        ThrowIfFailed(g_Device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&g_RTVHeap)));
        g_RTVDescriptorSize =
          g_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_RTVHeap->GetCPUDescriptorHandleForHeapStart());
        for (u32 i = 0; i < 2; i++) {
            ThrowIfFailed(g_SwapChain->GetBuffer(i, IID_PPV_ARGS(&g_RenderTargets[i])));
            g_Device->CreateRenderTargetView(g_RenderTargets[i].Get(), nullptr, rtvHandle);
            rtvHandle.Offset(1, g_RTVDescriptorSize);
        }

        // Create the command allocator and list
        ThrowIfFailed(g_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                       IID_PPV_ARGS(&g_CommandAllocator)));
        ThrowIfFailed(g_Device->CreateCommandList(0,
                                                  D3D12_COMMAND_LIST_TYPE_DIRECT,
                                                  g_CommandAllocator.Get(),
                                                  nullptr,
                                                  IID_PPV_ARGS(&g_CommandList)));

        // Close the command list since we will start in the recording state
        ThrowIfFailed(g_CommandList->Close());

        return true;
    }

    void GraphicsContext::RequestNewFrame() {
        static HANDLE fenceEvent = nullptr;
        static ComPtr<ID3D12Fence> fence;
        static u64 fenceValue = 0;

        if (!fence) {
            fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
            ThrowIfFailed(
              g_Device->CreateFence(fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));
        }

        const u64 currentValue = fenceValue;
        ThrowIfFailed(g_CommandQueue->Signal(fence.Get(), currentValue));
        fenceValue++;

        if (fence->GetCompletedValue() < currentValue) {
            ThrowIfFailed(fence->SetEventOnCompletion(currentValue, fenceEvent));
            WaitForSingleObject(fenceEvent, INFINITE);
        }
    }

    void GraphicsContext::BeginFrame() {
        // Reset command allocator and list
        ThrowIfFailed(g_CommandAllocator->Reset());
        ThrowIfFailed(g_CommandList->Reset(g_CommandAllocator.Get(), nullptr));

        // Set viewport and scissor rect
        D3D12_VIEWPORT viewport = {};
        auto [width, height]    = GetScreenSize();
        viewport.Width          = width;
        viewport.Height         = height;
        viewport.MinDepth       = D3D12_MIN_DEPTH;
        viewport.MaxDepth       = D3D12_MAX_DEPTH;
        g_CommandList->RSSetViewports(1, &viewport);

        D3D12_RECT scissorRect = {0, 0, CAST<i32>(width), CAST<i32>(height)};
        g_CommandList->RSSetScissorRects(1, &scissorRect);

        // Set render target
        const CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
          g_RTVHeap->GetCPUDescriptorHandleForHeapStart(),
          CAST<i32>(g_FrameIndex),
          g_RTVDescriptorSize);
        g_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

        // CLear the render target
        constexpr float clearColor[] = {100.f / 255.f,
                                        149.f / 255.f,
                                        237.f / 255.f,
                                        1.0f};  // Cornflower blue
        g_CommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

        // Record commands before calling EndFrame()
    }

    void GraphicsContext::EndFrame() {
        ThrowIfFailed(g_CommandList->Close());
        ID3D12CommandList* ppCommandLists[] = {g_CommandList.Get()};
        g_CommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

        ThrowIfFailed(g_SwapChain->Present(1, 0));
        g_FrameIndex = g_SwapChain->GetCurrentBackBufferIndex();
    }

    void GraphicsContext::Resize(const u32 width, const u32 height) {}

    Vector2 GetScreenSize() {
        RECT rc;
        ::GetClientRect(g_TargetHWND, &rc);
        return Vector2(CAST<f32>(rc.right - rc.left), CAST<f32>(rc.bottom - rc.top));
    }
}  // namespace Atom