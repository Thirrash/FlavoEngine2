#pragma once
#include "renderer_dx12_api.h"
#include "renderer_common/irenderer.h"
#include "core/ftl/memory.h"

namespace flavo::renderer::dx12::internal
{
	struct RendererDX12Data;
}

namespace flavo::renderer::dx12
{
	class RENDERER_DX12_API RendererDX12 : public IRenderer
	{
	public:
		RendererDX12();
		RendererDX12(const RendererDX12&) = default;
		RendererDX12(RendererDX12&&) noexcept = default;
		RendererDX12& operator=(const RendererDX12&) = default;
		RendererDX12& operator=(RendererDX12&&) noexcept = default;
		~RendererDX12();

		ftl::result<> Initialize(HWND window) override;
		ftl::result<> Destroy() override;
		ftl::result<> UpdateRender() override;
		ftl::result<> AddRenderItem(const RenderItem& /*ri*/) override { return ftl::make_error("TODO"); };

	private:
        ftl::unique_ptr<internal::RendererDX12Data> m_Data;
	};
}
