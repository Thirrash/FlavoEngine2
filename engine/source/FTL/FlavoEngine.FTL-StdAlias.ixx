module;

export module FlavoEngine.FTL:StdAlias;

import <string>;
import <vector>;

export namespace FlavoEngine::FTL
{
	template<typename T, typename Allocator=std::allocator<T>>
	using vector = std::vector<T, Allocator>;

	template<typename T, typename Allocator=std::allocator<T>>
	using basic_string = std::basic_string<T, std::char_traits<T>, Allocator>;
	using string = std::string;
	using string_view = std::string_view;
}
