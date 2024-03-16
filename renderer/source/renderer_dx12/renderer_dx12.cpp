#include "renderer_dx12_pch.h"
#include "renderer_dx12.h"
#include "core/logger/assert.h"

namespace flavo::renderer::dx12
{
	ftl::result<> RendererDX12::Initialize()
	{
		ftl::result<> device_result = InitializeDevice();
		RETURN_ON_ERROR(device_result, "Device initialization");

        ftl::result<> swapchain_result = InitializeSwapChain();
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
		return ftl::make_success();
	}

    ftl::result<> RendererDX12::InitializeSwapChain()
    {
        return ftl::make_success();
    }
}
