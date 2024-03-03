#pragma once
#include "core/ftl/string.h"
#include "result/result.h"

namespace flavo::ftl
{
	using void_t = result_sdk::unit_t;

	template<typename T = void_t, typename E = string>
	using result = result_sdk::Result<T, E>;

	template<typename T = void_t>
	using success = result_sdk::Ok<T>;

	template<typename E = string>
	using error = result_sdk::Err<E>;
}
