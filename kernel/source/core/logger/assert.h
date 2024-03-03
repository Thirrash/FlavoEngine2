#pragma once
#include "core/core_api.h"
#include "core/ftl/format.h"
#include "core/ftl/string.h"

namespace flavo::logger::impl
{
	CORE_API void HandleAssert(ftl::string_view msg, ftl::string_view file, int32_t line);
}

#define FLAVO_ASSERT(condition, format_msg, ...) \
	if (!condition) [[unlikely]]														\
	{																					\
		flavo::logger::impl::HandleAssert(flavo::ftl::format(format_msg, __VA_ARGS__), __FILE__, __LINE__);	\
	}
