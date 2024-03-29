#pragma once
#include "core_api.h"
#include "ftl/filesystem.h"

namespace flavo::core::path
{
	CORE_API ftl::filesystem::path GetBinaryPath();
	CORE_API ftl::filesystem::path GetOutputPath();
}
