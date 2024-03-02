#pragma once
#include "renderer_common_api.h"
#include "core/ftl/result.h"

namespace flavo::renderer
{
	struct RENDERER_COMMON_API RenderItem
	{
		float pos_x = 0.0f;
		float pos_y = 0.0f;
	};

	class RENDERER_COMMON_API IRenderer
	{
	public:
		virtual ftl::result<> Initialize() = 0;
		virtual ftl::result<> Destroy() = 0;

		virtual ftl::result<> AddRenderItem(const RenderItem& ri) = 0;
		virtual ftl::result<> ProcessFrame() = 0;
	};
}
