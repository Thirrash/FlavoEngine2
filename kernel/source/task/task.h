#pragma once
#include "task_api.h"

#include "concurrencpp/concurrencpp.h"

namespace flavo::task
{
	using Runtime = concurrencpp::runtime;
	template<class T> using Future = concurrencpp::result<T>;
	using BackgroundExecutor = concurrencpp::thread_pool_executor;
	using ThreadPoolExecutor = concurrencpp::thread_pool_executor;

	// Retrieve background executor, allowing scheduling more tasks
	// Background executor should be used for short tasks that are I/O bound or just blocking
	//   (e.g. file/socket read/write)
	TASK_API BackgroundExecutor& GetBackgroundExecutor();
	// Retrieve thread pool executor, allowing scheduling more tasks
	// Thread pool executor should be used for short tasks that are CPU-bound and non-blocking
	//  (e.g. computing stuff in parallel on a large array)
	TASK_API ThreadPoolExecutor& GetThreadPoolExecutor();

	// Needs to be called (ideally from 'main') before using any other function from flavo::task
	TASK_API void SetRuntime(Runtime& instance);
}
