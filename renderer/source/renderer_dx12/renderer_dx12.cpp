#include "renderer_dx12_pch.h"
#include "renderer_dx12.h"
#include "core/logger/assert.h"

#include <dxgi1_4.h>

namespace flavo::renderer::dx12
{
	ftl::result<> RendererDX12::Initialize(uint32_t window_width, uint32_t window_height, HWND window)
	{
		ftl::result<> device_result = InitializeDevice();
		RETURN_ON_ERROR(device_result, "Device initialization");

        ftl::result<> command_queue_result = InitializeCommandQueue();
        RETURN_ON_ERROR(command_queue_result, "Command queue initialization");

        ftl::result<> swapchain_result = InitializeSwapChain(window_width, window_height, window);
        RETURN_ON_ERROR(swapchain_result, "Swapchain initialization");

		return ftl::make_success();
	}

	ftl::result<> RendererDX12::Destroy()
	{
		return ftl::make_success();
	}

	ftl::result<> RendererDX12::UpdateRender()
	{
		return ftl::make_success();
	}

	ftl::result<> RendererDX12::InitializeDevice()
	{
        HRESULT hr;

        IDXGIFactory4* dxgi_factory;
        hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgi_factory));
        if (FAILED(hr))
            return ftl::make_error(ftl::format("CreateDXGIFactory1 failed with code: {}", hr));

        // Find GPU supporting DX12
        // Dedicated GPU has priority but we still allow software on, if nothing else is present
        ftl::vector<IDXGIAdapter1*> dedicated_adapters;
        ftl::vector<IDXGIAdapter1*> software_adapters;

        IDXGIAdapter1* adapter = nullptr;
        uint32_t adapter_idx = 0;
        while (dxgi_factory->EnumAdapters1(adapter_idx, &adapter) != DXGI_ERROR_NOT_FOUND)
        {
            ++adapter_idx;

            DXGI_ADAPTER_DESC1 desc;
            adapter->GetDesc1(&desc);

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                software_adapters.push_back(adapter);
            }
            else
            {
                dedicated_adapters.push_back(adapter);
            }
        }

        bool found_adapter = false;
        auto try_adapter = [](IDXGIAdapter1* current_adapter, ID3D12Device*& device) -> bool
        {
            HRESULT hr = D3D12CreateDevice(current_adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));
            return SUCCEEDED(hr);
        };

        // Try dedicated first
        for (IDXGIAdapter1* adap : dedicated_adapters)
        {
            if (try_adapter(adap, m_Device))
                return ftl::make_success();
        }

        // Try software as a fallback
        for (IDXGIAdapter1* adap : software_adapters)
        {
            if (try_adapter(adap, m_Device))
                return ftl::make_success();
        }

        return ftl::make_error("Couldn't find any compatible adapter");
	}

    ftl::result<> RendererDX12::InitializeCommandQueue()
    {
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 0;

        HRESULT hr = m_Device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_CommandQueue));
        if (FAILED(hr))
            return ftl::make_error(ftl::format("CreateCommandQueue failed with code: {}", hr));

        return ftl::make_success();
    }

    ftl::result<> RendererDX12::InitializeSwapChain(uint32_t window_width, uint32_t window_height, HWND window)
    {
        constexpr bool fullscreen = false;

        // Describe our display mode
        DXGI_MODE_DESC back_buffer_desc = {}; 
        back_buffer_desc.Width = window_width;
        back_buffer_desc.Height = window_height;
        back_buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

        // Describe our multi-sampling. We are not multi-sampling, so we set the count to 1 (we need at least one sample of course)
        DXGI_SAMPLE_DESC sample_desc = {};
        sample_desc.Count = 1;

        // Describe and create the swap chain.
        DXGI_SWAP_CHAIN_DESC desc = {};
        desc.BufferCount = NUM_FRAME_BUFFERS;
        desc.BufferDesc = back_buffer_desc;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // this says the pipeline will render to this swap chain
        desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // dxgi will discard the buffer (data) after we call present
        desc.OutputWindow = window; // handle to our window
        desc.SampleDesc = sample_desc; // our multi-sampling description
        desc.Windowed = !fullscreen; // set to true, then if in fullscreen must call SetFullScreenState with true for full screen to get uncapped fps

        IDXGIFactory4* dxgi_factory = nullptr;
        HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&dxgi_factory));
        if (FAILED(hr))
            return ftl::make_error(ftl::format("CreateDXGIFactory1 failed with code: {}", hr));

        IDXGISwapChain* tmp_swapchain = nullptr;
        hr = dxgi_factory->CreateSwapChain
        (
            m_CommandQueue, // the queue will be flushed once the swap chain is created
            &desc, // give it the swap chain description we created above
            &tmp_swapchain // store the created swap chain in a temp IDXGISwapChain interface
        );
        if (FAILED(hr))
            return ftl::make_error(ftl::format("CreateSwapChain failed with code: {}", hr));

        m_SwapChain = static_cast<IDXGISwapChain3*>(tmp_swapchain);
        m_CurrentRenderTargetIdx = m_SwapChain->GetCurrentBackBufferIndex();

        return ftl::make_success();
    }
}
