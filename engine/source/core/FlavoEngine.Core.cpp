module FlavoEngine.Core;

namespace FlavoEngine::Core
{
	std::filesystem::path Paths::GetBinaryPath()
	{
		return std::filesystem::current_path();
	}

	std::filesystem::path Paths::GetOutputPath()
	{
		wchar_t appdata_path[256];
		size_t required_size;
		const errno_t err_code = _wgetenv_s(&required_size, appdata_path, L"APPDATA");
		if (err_code != 0)
			return std::filesystem::path();

		const std::filesystem::path out_path = std::filesystem::path(appdata_path) / "FlavoTeam";
		std::filesystem::create_directories(out_path);
		return out_path;
	}
}
