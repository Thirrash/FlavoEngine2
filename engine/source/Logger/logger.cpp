#include "logger/logger.h"
#include "core/core.h"
#include "core/ftl/chrono.h"
#include "core/ftl/format.h"
#include "core/ftl/fstream.h"
#include "core/ftl/iostream.h"

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
		const ftl::string str = ftl::format("{} [{}] {}\n", GetCurrentDateTime(), type, message);
		ftl::cout << str;

		ftl::ofstream file_handle(m_FilePath, ftl::ios::app);
		if (file_handle.is_open())
		{
			file_handle << str;
		}
	}
}
