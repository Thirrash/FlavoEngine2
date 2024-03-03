#pragma once
#include "core/ftl/format.h"
#include "core/ftl/string.h"
#include "core/ftl/type_traits.h"
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

	template<typename T>
	success<T> make_success(const T& val)
	{
		return success<T>(val);
	}

	template<typename T>
	success<T> make_success(T&& val)
	{
		return success<T>(val);
	}

	success<void_t> make_success()
	{
		return success<void_t>();
	}

	template<typename E>
	error<E> make_error(const E& val)
	{
		return error<E>(val);
	}

	template<typename E>
	error<E> make_error(E&& val)
	{
		return error<E>(val);
	}

	template<typename ...Args>
	error<ftl::string> make_error(ftl::string_view fmt, Args&&... args)
	{
		return error<ftl::string>(ftl::vformat(fmt, ftl::make_format_args(ftl::forward<Args>(args)...)));
	}
}
