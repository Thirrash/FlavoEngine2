module FlavoEngine.Logger;

import FlavoEngine.Core;

namespace FlavoEngine::Logger
{
	LogEngine& LogEngine::GetInstance()
	{
		static LogEngine instance;
		return instance;
	}

	std::string LogEngine::GetCurrentDateTime()
	{
		auto now = std::chrono::system_clock::now();
		return std::format("{:%d-%m-%Y %H:%M:%S}", now);
	}
	
	LogEngine::LogEngine()
	{
		const std::filesystem::path path_suffix(L"Logs");
		std::filesystem::path log_dir = FlavoEngine::Core::Paths::GetOutputPath() / path_suffix;
		const std::string filename = std::format("log_{}", GetCurrentDateTime());
		m_FilePath = log_dir / filename;
	}

	void LogEngine::LogInternal(std::string_view type, std::string_view message) const
	{
		const std::string str = std::format("{} [{}] {}\n", GetCurrentDateTime(), type, message);
		std::cout << str;

		std::ofstream file_handle(m_FilePath, std::ios::app);
		if (file_handle.is_open())
		{
			file_handle << str;
		}
	}
}
