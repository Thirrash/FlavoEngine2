#pragma once
#include "core/core_api.h"
#include <string>

namespace flavo::ftl
{
	using std::string;
	using std::string_view;
	using std::wstring;
	using std::wstring_view;

	CORE_API string utf16_to_utf8(wstring_view s);
	CORE_API wstring utf8_to_utf16(string_view s);
}
