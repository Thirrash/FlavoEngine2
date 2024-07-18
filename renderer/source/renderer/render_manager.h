#pragma once
#include "renderer_api.h"
#include "renderer_common/irenderer.h"
#include "core/ftl/memory.h"
#include "core/parallel/task.h"
#include "core/platform/os_windows.h"

namespace flavo::renderer
{
	enum class ERendererType
	{
		Dummy = 0,
		DX12
	};

	class RENDERER_API RenderManager
	{
	public:
		ftl::result<> Initialize(ERendererType rtype, HWND window);
		ftl::result<> Destroy();
		parallel::Future<ftl::result<>> UpdateRender();

	private:
		ftl::unique_ptr<IRenderer> m_Renderer;
	};

	RenderManager g_RenderManager;
}
