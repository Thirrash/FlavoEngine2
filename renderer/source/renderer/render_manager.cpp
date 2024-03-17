#include "render_manager.h"
#include "renderer_dx12/renderer_dx12.h"
#include "core/ftl/format.h"
#include "core/ftl/utility.h"

namespace flavo::renderer
{
	ftl::result<> RenderManager::Initialize(ERendererType rtype, HWND window)
	{
		switch (rtype)
		{
		case ERendererType::DX12:
			m_Renderer = ftl::make_unique<dx12::RendererDX12>();
			break;
		default:
			return ftl::make_error(ftl::format("Not supported renderer type: {}", ftl::to_underlying(rtype)));
		}

		return m_Renderer->Initialize(window);
	}

	ftl::result<> RenderManager::Destroy()
	{
		return m_Renderer->Destroy();
	}

	ftl::result<> RenderManager::UpdateRender()
	{
		return m_Renderer->UpdateRender();
	}
}
