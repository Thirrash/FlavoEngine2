#pragma once
#include "renderer_dx12_api.h"
#include "renderer_common/irenderer.h"

namespace flavo::renderer::dx12
{
	class RENDERER_DX12_API RendererDX12 : public IRenderer
	{
		ftl::result<> Initialize() override;
		ftl::result<> Destroy() override;

		ftl::result<> AddRenderItem(const RenderItem& /*ri*/) override { return ftl::error<>("TODO"); };
		ftl::result<> ProcessFrame() override { return ftl::error<>("TODO"); };
	};
}
