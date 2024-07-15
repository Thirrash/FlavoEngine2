#pragma once
#include "renderer_api.h"
#include "renderer_common/irenderer.h"
#include "core/ftl/memory.h"
#include "core/platform/os_windows.h"
#include "task/task.h"

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
		ftl::result<> Initialize(ERendererType rtype);
		ftl::result<> Destroy();
		task::Future<ftl::result<>> UpdateRender();

	private:
		ftl::unique_ptr<IRenderer> m_Renderer;
	};

	RenderManager g_RenderManager;
}
