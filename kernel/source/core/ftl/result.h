#pragma once
#include "core/ftl/format.h"
#include "core/ftl/source_location.h"
#include "core/ftl/string.h"
#include "core/ftl/type_traits.h"
#include "core/ftl/utility.h"
#include "core/ftl/vector.h"
#include "result/result.h"

namespace flavo::ftl::internal
{
	string make_error_message(string_view msg, source_location src_location)
	{
		string file_name;

		string_view file_name_view = src_location.file_name();
		auto&& slash_it = file_name_view.rfind('/');
		if (slash_it != file_name_view.npos)
		{
			file_name = file_name_view.substr(slash_it + 1);
		}
		else
		{
			auto&& backslash_it = file_name_view.rfind('\\');
			if (backslash_it != file_name_view.npos)
			{
				file_name = file_name_view.substr(backslash_it + 1);
			}
			else
			{
				file_name = file_name_view;
			}
		}
			
		return format("{} [{}({})]", msg, file_name, src_location.line());
	}
}

namespace flavo::ftl
{
	template<typename E>
	class error_type
	{
	public:
		explicit constexpr error_type(const E& val, string_view msg) 
			: m_Data(val)
		{
			m_Messages.push_back(msg.data());
		}
		explicit constexpr error_type(E&& val, string_view msg)
			: m_Data(move(val))
		{
			m_Messages.push_back(msg.data());
		}
		template<typename U>
		explicit constexpr error_type(const U& val, string_view msg)
			: m_Data(val)
		{
			m_Messages.push_back(msg.data());
		}
		template<typename U>
		explicit constexpr error_type(U&& val, string_view msg)
			: m_Data(move(val))
		{
			m_Messages.push_back(msg.data());
		}

		constexpr const E& data() const& { return m_Data; }
		constexpr E&& data() && { return std::move(m_Data); }

		constexpr const vector<string>& messages() const& { return m_Messages; }
		constexpr vector<string>&& messages() && { return std::move(m_Messages); }

		error_type<E>& add_message(string_view msg, source_location src_location = source_location::current())
		{ 
			m_Messages.push_back(internal::make_error_message(msg, src_location));
			return *this;
		}

		string join_messages() const
		{
			string tmp;
			for (const string& s : m_Messages)
			{
				tmp.append(s);
				tmp.append("\n");
			}

			if (!tmp.empty())
			{
				tmp.pop_back();
			}

			return tmp;
		}

	private:
		E m_Data;
		vector<string> m_Messages;
	};

	using void_t = result_sdk::unit_t;

	template<typename T = void_t, typename E = void_t>
	using result = result_sdk::Result<T, error_type<E>>;

	template<typename T = void_t>
	using success = result_sdk::Ok<T>;

	template<typename E = void_t>
	using error = result_sdk::Err<error_type<E>>;

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

	template<typename E, typename ...Args>
	error<E> make_error(const E& val, string_view msg, source_location src_location = source_location::current())
	{
		return error<E>(error_type<E>(val, internal::make_error_message(msg, src_location)));
	}

	template<typename E, typename ...Args>
	error<E> make_error(E&& val, string_view msg, source_location src_location = source_location::current())
	{
		return error<E>(error_type<E>(move(val), internal::make_error_message(msg, src_location)));
	}

	template<typename ...Args>
	error<void_t> make_error(string_view msg, source_location src_location = source_location::current())
	{
		return error<void_t>(error_type<void_t>(void_t(), internal::make_error_message(msg, src_location)));
	}
}

#define RETURN_ON_ERROR(res, fmt, ...) { if (res.is_err()) { return flavo::ftl::error(res.err_unchecked().add_message(flavo::ftl::format(fmt, __VA_ARGS__))); } }
