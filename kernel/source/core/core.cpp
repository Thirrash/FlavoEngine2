#include "core/core.h"

namespace flavo::path
{
	ftl::filesystem::path GetBinaryPath()
	{
		return ftl::filesystem::current_path();
	}

	ftl::filesystem::path GetOutputPath()
	{
		wchar_t appdata_path[256];
		size_t required_size;
		const errno_t err_code = _wgetenv_s(&required_size, appdata_path, L"APPDATA");
		if (err_code != 0)
			return ftl::filesystem::path();

		const ftl::filesystem::path out_path = ftl::filesystem::path(appdata_path) / "FlavoEntertainment";
		ftl::filesystem::create_directories(out_path);
		return out_path;
	}
}
