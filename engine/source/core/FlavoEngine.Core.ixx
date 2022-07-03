export module FlavoEngine.Core;

import std.filesystem;

namespace FlavoEngine::Core
{
	export class __declspec(dllexport) Paths
	{
	public:
		static std::filesystem::path GetBinaryPath();
		static std::filesystem::path GetOutputPath();
	};
}
