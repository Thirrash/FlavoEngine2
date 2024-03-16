#pragma once
#include "core/core_api.h"
#include "core/ftl/format.h"
#include "core/ftl/source_location.h"
#include "core/ftl/string.h"

namespace flavo::logger::internal
{
	CORE_API void HandleAssert(ftl::string_view msg, ftl::source_location src_location);
}

#define FLAVO_ASSERT(condition, format_msg, ...) \
	{ \
		if (!condition) [[unlikely]] \
		{ \
			flavo::logger::internal::HandleAssert(flavo::ftl::format(format_msg, __VA_ARGS__), flavo::ftl::source_location::current());	\
		} \
	}
