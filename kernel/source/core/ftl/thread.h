#pragma once
#include <thread>

namespace flavo::ftl
{
	namespace this_thread
	{
		using std::this_thread::get_id;
		using std::this_thread::sleep_for;
	}

	using std::thread;
}
