#include "string.h"
#include <codecvt>

namespace flavo::ftl
{
	string utf16_to_utf8(wstring_view s)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.to_bytes(s.data());
	}

	wstring utf8_to_utf16(string_view s)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		return conv.from_bytes(s.data());
	}
}
