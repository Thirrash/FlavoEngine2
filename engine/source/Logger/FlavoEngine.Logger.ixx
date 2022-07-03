export module FlavoEngine.Logger;

// Note: due to bug in VS2022, filesystem needs to be imported before core to allow string->path conversion
import std.filesystem;
import std.core;

namespace FlavoEngine::Logger
{
	class __declspec(dllexport) LogEngine
	{
	public:
		static LogEngine& GetInstance();

		template<typename ...Args>
		void Log(std::string_view type, std::string_view format, Args&&... args) const
		{
			const std::string message = std::vformat(format, std::make_format_args(std::forward<Args>(args)...));
			LogInternal(type, message);
		}

	private:
		static std::string GetCurrentDateTime();

		LogEngine();
		LogEngine(const LogEngine&) = delete;
		LogEngine(LogEngine&&) = delete;
		LogEngine& operator=(const LogEngine&) = delete;
		LogEngine& operator=(LogEngine&&) = delete;

		void LogInternal(std::string_view type, std::string_view message) const;

		std::filesystem::path m_FilePath;
	};

	// Log debug message
	export template<typename ...Args>
	void debug(std::string_view format, Args&&... args)
	{
		LogEngine::GetInstance().Log("DEBG", format, std::forward<Args>(args)...);
	}

	// Log info message
	export template<typename ...Args>
	void info(std::string_view format, Args&&... args)
	{
		LogEngine::GetInstance().Log("INFO", format, std::forward<Args>(args)...);
	}

	// Log warning message
	export template<typename ...Args>
	void warning(std::string_view format, Args&&... args)
	{
		LogEngine::GetInstance().Log("WARN", format, std::forward<Args>(args)...);
	}

	// Log error message
	export template<typename ...Args>
	void error(std::string_view format, Args&&... args)
	{
		LogEngine::GetInstance().Log("ERRO", format, std::forward<Args>(args)...);
	}
}
