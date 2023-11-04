#include "core/logger/logger.h"

#include "core/core.h"
#include "core/ftl/chrono.h"
#include "core/ftl/format.h"
#include "core/ftl/fstream.h"
#include "core/ftl/iostream.h"
#include "core/ftl/thread.h"
#include "core/parallel/thread_name.h"

#ifdef _MSC_VER
#include <windows.h>
#include <debugapi.h>
#endif

namespace flavo::logger
{
	LogEngine& LogEngine::GetInstance()
	{
		static LogEngine instance;
		return instance;
	}

	ftl::string LogEngine::GetCurrentDateTime()
	{
		auto now = ftl::chrono::system_clock::now();
		return ftl::format("{:%H:%M:%S}", now);
	}
	
	LogEngine::LogEngine()
	{
		const ftl::filesystem::path path_suffix(L"Logs");
		ftl::filesystem::path log_dir = core::path::GetOutputPath() / path_suffix;
		const ftl::string filename = ftl::format("log_{}", GetCurrentDateTime());
		m_FilePath = log_dir / filename;
	}

	void LogEngine::LogInternal(ftl::string_view type, ftl::string_view message) const
	{
		const ftl::thread::id thread_id = ftl::this_thread::get_id();
		const ftl::string_view thread_name = parallel::ThreadNameManager::Instance().GetName(thread_id);
		const ftl::string str = ftl::format("{} [{}] [{}] {}\n", GetCurrentDateTime(), thread_name, type, message);
		ftl::cout << str;

		ftl::ofstream file_handle(m_FilePath, ftl::ios::app);
		if (file_handle.is_open())
		{
			ftl::scoped_lock l(m_Lock);
			file_handle << str;
		}

#ifdef _MSC_VER
		OutputDebugStringA(str.c_str());
#endif
	}
}
