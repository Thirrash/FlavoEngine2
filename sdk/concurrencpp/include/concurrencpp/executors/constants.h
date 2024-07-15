#ifndef CONCURRENCPP_EXECUTORS_CONSTS_H
#define CONCURRENCPP_EXECUTORS_CONSTS_H

#include <limits>
#include <numeric>

namespace concurrencpp::details::consts {
    inline const char* k_inline_executor_name = "Inline";
    constexpr int k_inline_executor_max_concurrency_level = 0;

    inline const char* k_thread_executor_name = "Thread";
    constexpr int k_thread_executor_max_concurrency_level = std::numeric_limits<int>::max();

    inline const char* k_thread_pool_executor_name = "ThreadPool";
    inline const char* k_background_executor_name = "Background";

    constexpr int k_worker_thread_max_concurrency_level = 1;
    inline const char* k_worker_thread_executor_name = "WorkerThread";

    inline const char* k_manual_executor_name = "ManualExec";
    constexpr int k_manual_executor_max_concurrency_level = std::numeric_limits<int>::max();

    inline const char* k_timer_queue_name = "TimerQueue";

    inline const char* k_executor_shutdown_err_msg = " - shutdown has been called on this executor.";
}  // namespace concurrencpp::details::consts

#endif
