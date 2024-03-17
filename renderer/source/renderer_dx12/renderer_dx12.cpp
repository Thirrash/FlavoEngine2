#include "renderer_dx12_pch.h"
#include "renderer_dx12.h"
#include "core/logger/assert.h"

#include "d3dx12/d3dx12.h"
#include <dxgi1_4.h>

namespace flavo::renderer::dx12::internal
{
    ftl::result<> InitializeDevice(ID3D12Device*& out_device)
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
            if (try_adapter(adap, out_device))
                return ftl::make_success();
        }

        // Try software as a fallback
        for (IDXGIAdapter1* adap : software_adapters)
        {
            if (try_adapter(adap, out_device))
                return ftl::make_success();
        }

        return ftl::make_error("Couldn't find any compatible adapter");
    }

    ftl::result<> InitializeCommandQueue(ID3D12Device& device, ID3D12CommandQueue*& out_cmd_queue)
    {
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        desc.NodeMask = 0;

        HRESULT hr = device.CreateCommandQueue(&desc, IID_PPV_ARGS(&out_cmd_queue));
        if (FAILED(hr))
            return ftl::make_error(ftl::format("CreateCommandQueue failed with code: {}", hr));

        return ftl::make_success();
    }

    ftl::result<> InitializeSwapChain(HWND window, uint32_t num_frame_buffers, ID3D12CommandQueue& cmd_queue, IDXGISwapChain3*& out_swap_chain, uint8_t& out_rtv_idx)
    {
        constexpr bool fullscreen = false;

        // Describe our display mode
        DXGI_MODE_DESC back_buffer_desc = {};
        back_buffer_desc.Width = 0; // use the same as window
        back_buffer_desc.Height = 0; // use the same as window
        back_buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

        // Describe our multi-sampling. We are not multi-sampling, so we set the count to 1 (we need at least one sample of course)
        DXGI_SAMPLE_DESC sample_desc = {};
        sample_desc.Count = 1;

        // Describe and create the swap chain.
        DXGI_SWAP_CHAIN_DESC desc = {};
        desc.BufferCount = num_frame_buffers;
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
            &cmd_queue, // the queue will be flushed once the swap chain is created
            &desc, // give it the swap chain description we created above
            &tmp_swapchain // store the created swap chain in a temp IDXGISwapChain interface
        );
        if (FAILED(hr))
            return ftl::make_error(ftl::format("CreateSwapChain failed with code: {}", hr));

        out_swap_chain = static_cast<IDXGISwapChain3*>(tmp_swapchain);
        out_rtv_idx = static_cast<uint8_t>(out_swap_chain->GetCurrentBackBufferIndex());

        return ftl::make_success();
    }

    ftl::result<> InitializeRTVDescriptorHeap(uint32_t num_frame_buffers, ID3D12Device& device, ID3D12DescriptorHeap*& out_rtv_desc_heap, uint64_t& out_rtv_desc_size)
    {
        D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
        rtvHeapDesc.NumDescriptors = num_frame_buffers; // number of descriptors for this heap.
        rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // this heap is a render target view heap

        // This heap will not be directly referenced by the shaders (not shader visible), as this will store the output from the pipeline
        rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        HRESULT hr = device.CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&out_rtv_desc_heap));
        if (FAILED(hr))
            return ftl::make_error(ftl::format("CreateDescriptorHeap failed with code: {}", hr));

        // get the size of a descriptor in this heap (this is a rtv heap, so only rtv descriptors should be stored in it.
        // descriptor sizes may vary from device to device, which is why there is no set size and we must ask the 
        // device to give us the size. we will use this size to increment a descriptor handle offset
        out_rtv_desc_size = device.GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        return ftl::make_success();
    }

    ftl::result<> InitializeRenderTargets(uint32_t num_frame_buffers, uint64_t rtv_desc_size, ID3D12DescriptorHeap& rtv_desc_heap, IDXGISwapChain3& swapchain, ID3D12Device& device, ID3D12Resource** out_render_targets)
    {
        // get a handle to the first descriptor in the descriptor heap. a handle is basically a pointer,
        // but we cannot literally use it like a c++ pointer.
        CD3DX12_CPU_DESCRIPTOR_HANDLE rtv_desc_heap_handle(rtv_desc_heap.GetCPUDescriptorHandleForHeapStart());

        // Create a RTV for each frame buffer
        for (uint32_t i = 0; i < num_frame_buffers; ++i)
        {
            // first we get the n'th buffer in the swap chain and store it in the n'th
            // position of our ID3D12Resource array
            HRESULT hr = swapchain.GetBuffer(i, IID_PPV_ARGS(&out_render_targets[i]));
            if (FAILED(hr))
                return ftl::make_error(ftl::format("IDXGISwapChain3::GetBuffer failed with code: {} for buffer: {}", hr, i));

            // the we "create" a render target view which binds the swap chain buffer (ID3D12Resource[n]) to the rtv handle
            device.CreateRenderTargetView(out_render_targets[i], nullptr, rtv_desc_heap_handle);

            // we increment the rtv handle by the rtv descriptor size we got above
            rtv_desc_heap_handle.Offset(1, rtv_desc_size);
        }

        return ftl::make_success();
    }

    ftl::result<> InitializeCommandAllocators(uint32_t num_frame_buffers, ID3D12Device& device, ID3D12CommandAllocator** out_cmd_allocators)
    {
        for (uint32_t i = 0; i < num_frame_buffers; ++i)
        {
            HRESULT hr = device.CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&out_cmd_allocators[i]));
            if (FAILED(hr))
                return ftl::make_error(ftl::format("CreateCommandAllocator failed with code: {} for allocator: {}", hr, i));
        }

        return ftl::make_success();
    }

    ftl::result<> InitializeCommandLists(uint32_t num_threads, ID3D12Device& device, ID3D12CommandAllocator& cmd_allocator, ID3D12GraphicsCommandList** out_command_lists)
    {
        for (uint32_t i = 0; i < num_threads; ++i)
        {
            // create the command list with the first allocator
            HRESULT hr = device.CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, &cmd_allocator, nullptr, IID_PPV_ARGS(&out_command_lists[i]));
            if (FAILED(hr))
                return ftl::make_error(ftl::format("CreateCommandList failed with code: {} for list: {}", hr, i));

            // command lists are created in the recording state. our main loop will set it up for recording again so close it now
            out_command_lists[i]->Close();
        }

        return ftl::make_success();
    }
}

namespace flavo::renderer::dx12
{
	ftl::result<> RendererDX12::Initialize(HWND window)
	{
		ftl::result<> device_result = internal::InitializeDevice(m_Device);
		RETURN_ON_ERROR(device_result, "Device initialization");

        ftl::result<> command_queue_result = internal::InitializeCommandQueue(*m_Device, m_CommandQueue);
        RETURN_ON_ERROR(command_queue_result, "Command queue initialization");

        ftl::result<> swapchain_result = internal::InitializeSwapChain(window, NUM_FRAME_BUFFERS, *m_CommandQueue, m_SwapChain, m_CurrentRenderTargetIdx);
        RETURN_ON_ERROR(swapchain_result, "Swapchain initialization");

        ftl::result<> rtv_descriptor_result = internal::InitializeRTVDescriptorHeap(NUM_FRAME_BUFFERS, *m_Device, m_RTVDescriptorHeap, m_RTVDescriptorSize);
        RETURN_ON_ERROR(rtv_descriptor_result, "RTV descriptor heap initialization");

        ftl::result<> render_targets_result = internal::InitializeRenderTargets(NUM_FRAME_BUFFERS, m_RTVDescriptorSize, *m_RTVDescriptorHeap, *m_SwapChain, *m_Device, m_RenderTargets);
        RETURN_ON_ERROR(render_targets_result, "Render targets initialization");

        ftl::result<> command_allocator_result = internal::InitializeCommandAllocators(NUM_FRAME_BUFFERS, *m_Device, m_CommandAllocators);
        RETURN_ON_ERROR(command_allocator_result, "Command allocators initialization");

        ftl::result<> command_list_result = internal::InitializeCommandLists(NUM_RENDER_THREADS, *m_Device, *m_CommandAllocators[0], m_CommandLists);
        RETURN_ON_ERROR(command_list_result, "Command lists initialization");

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
}
