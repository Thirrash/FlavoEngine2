#pragma once

#include "core/core_api.h"

#include "concurrencpp/concurrencpp.h"

namespace flavo::parallel
{
	using Runtime = concurrencpp::runtime;
	using RuntimeOptions = concurrencpp::runtime_options;
	template<class T> using Future = concurrencpp::result<T>;
	template<class T> using LazyFuture = concurrencpp::lazy_result<T>;
	using BackgroundExecutor = concurrencpp::thread_pool_executor;
	using ThreadPoolExecutor = concurrencpp::thread_pool_executor;

	// Retrieve background executor, allowing scheduling more tasks
	// Background executor should be used for short tasks that are I/O bound or just blocking
	//   (e.g. file/socket read/write)
	CORE_API BackgroundExecutor& GetBackgroundExecutor();
	// Retrieve thread pool executor, allowing scheduling more tasks
	// Thread pool executor should be used for short tasks that are CPU-bound and non-blocking
	//  (e.g. computing stuff in parallel on a large array)
	CORE_API ThreadPoolExecutor& GetThreadPoolExecutor();

	// Needs to be called (ideally from 'main') before using any other function from flavo::task
	CORE_API void SetRuntime(Runtime& instance);
}
