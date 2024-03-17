#pragma once
#include "renderer_dx12_api.h"
#include "renderer_common/irenderer.h"

struct ID3D12CommandAllocator;
struct ID3D12CommandQueue;
struct ID3D12DescriptorHeap;
struct ID3D12Device;
struct ID3D12Fence;
struct ID3D12GraphicsCommandList;
struct ID3D12Resource;
struct IDXGISwapChain3;

namespace flavo::renderer::dx12
{
	class RENDERER_DX12_API RendererDX12 : public IRenderer
	{
	public:
		ftl::result<> Initialize(HWND window) override;
		ftl::result<> Destroy() override;

		ftl::result<> UpdateRender() override;

		ftl::result<> AddRenderItem(const RenderItem& /*ri*/) override { return ftl::make_error("TODO"); };

	private:
        // Triple-buffering, number of RTVs
        static constexpr uint32_t NUM_FRAME_BUFFERS = 3;
        // Number of threads adding render commands
        static constexpr uint32_t NUM_RENDER_THREADS = 1;

        // Direct3D device - most basic class corresponding to the adapter (GPU)
        ID3D12Device* m_Device = nullptr;
        // Swapchain allows to switch between multiple render targets (frame buffers)
        IDXGISwapChain3* m_SwapChain = nullptr;
        // Current rtv we are on, always in range [0, NUM_FRAME_BUFFERS)
        uint8_t m_CurrentRenderTargetIdx = 0;
        // Allows to create and execute command lists
        ID3D12CommandQueue* m_CommandQueue = nullptr;
        // Container for render targets (RTV)
        ID3D12DescriptorHeap* m_RTVDescriptorHeap = nullptr;
        // Size of the rtv descriptor on the device (all front and back buffers will be the same size)
        uint64_t m_RTVDescriptorSize = 0;
        // Render targets
        ID3D12Resource* m_RenderTargets[NUM_FRAME_BUFFERS] = {};
        // Allocators for render commands, those cannot be reset until waiting for a fence event
        ID3D12CommandAllocator* m_CommandAllocators[NUM_FRAME_BUFFERS] = {};
        // A command list we can record commands into, then execute them to render the frame
        ID3D12GraphicsCommandList* m_CommandLists[NUM_RENDER_THREADS] = {};
        // An object that is locked while our command list is being executed by the gpu. We need as many 
        // as we have allocators (more if we want to know when the gpu is finished with an asset)
        ID3D12Fence* m_Fences[NUM_FRAME_BUFFERS][NUM_RENDER_THREADS] = {};
        // A handle to an event when our fence is unlocked by the gpu
        void* m_FenceEvents[NUM_RENDER_THREADS] = {};
        // This value is incremented each frame. Each fence will have its own value
        uint64_t m_FenceValue[NUM_FRAME_BUFFERS][NUM_RENDER_THREADS] = {}; 
	};
}
