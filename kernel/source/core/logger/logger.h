#pragma once
#include "core/core_api.h"
#include "core/ftl/filesystem.h"
#include "core/ftl/format.h"
#include "core/ftl/mutex.h"
#include "core/ftl/string.h"
#include "core/ftl/utility.h"

namespace flavo::logger
{
	class CORE_API LogEngine
	{
	public:
		static LogEngine& GetInstance();

		template<typename ...Args>
		void Log(ftl::string_view type, ftl::string_view format, Args&&... args) const
		{
			const ftl::string message = ftl::vformat(format, ftl::make_format_args(ftl::forward<Args>(args)...));
			LogInternal(type, message);
		}

	private:
		static ftl::string GetCurrentDateTime();

		LogEngine();
		LogEngine(const LogEngine&) = delete;
		LogEngine(LogEngine&&) = delete;
		LogEngine& operator=(const LogEngine&) = delete;
		LogEngine& operator=(LogEngine&&) = delete;

		void LogInternal(ftl::string_view type, ftl::string_view message) const;

		ftl::filesystem::path m_FilePath;
		mutable ftl::mutex m_Lock;
	};

	// Log debug message
	template<typename ...Args>
	void debug(ftl::string_view format, Args&&... args)
	{
		LogEngine::GetInstance().Log("DEBG", format, ftl::forward<Args>(args)...);
	}

	// Log info message
	template<typename ...Args>
	void info(ftl::string_view format, Args&&... args)
	{
		LogEngine::GetInstance().Log("INFO", format, ftl::forward<Args>(args)...);
	}

	// Log warning message
	template<typename ...Args>
	void warning(ftl::string_view format, Args&&... args)
	{
		LogEngine::GetInstance().Log("WARN", format, ftl::forward<Args>(args)...);
	}

	// Log error message
	template<typename ...Args>
	void error(ftl::string_view format, Args&&... args)
	{
		LogEngine::GetInstance().Log("ERRO", format, ftl::forward<Args>(args)...);
	}
}
