#include "task.h"

#include "core/logger/assert.h"

namespace flavo::parallel::internal
{
	Runtime* g_Runtime = nullptr;
}

namespace flavo::parallel
{
	BackgroundExecutor& GetBackgroundExecutor()
	{
		FLAVO_ASSERT(internal::g_Runtime != nullptr, "Flavo task system runtime not initialized");
		return *internal::g_Runtime->background_executor_raw();
	}

	ThreadPoolExecutor& GetThreadPoolExecutor()
	{
		FLAVO_ASSERT(internal::g_Runtime != nullptr, "Flavo task system runtime not initialized");
		return *internal::g_Runtime->thread_pool_executor_raw();
	}

	void SetRuntime(Runtime& instance)
	{
		FLAVO_ASSERT(internal::g_Runtime == nullptr, "Flavo task system runtime already initialized");
		internal::g_Runtime = &instance;
	}
}