#pragma once

#include "core/core_api.h"
#include "core/ftl/future.h"
#include "core/ftl/memory.h"

namespace flavo::parallel
{
    class TaskManagerImpl;

    struct TaskParams
    {
        // Caller is responsible for memory management if user data is heap-allocated
        void* user_data = nullptr;
    };

    using TaskFunction = void(*)(TaskParams);

    class CORE_API TaskManager
    {
    public:
        static TaskManager& Instance();

        TaskManager();
        TaskManager(const TaskManager&) = delete;
        TaskManager(TaskManager&&) = delete;
        TaskManager& operator=(const TaskManager&) = delete;
        TaskManager& operator=(TaskManager&&) = delete;
        ~TaskManager();

        /// Create a new task which could be queried and waited for
        // More expensive than `SpawnDetachedTask`
        ftl::future<void> SpawnWaitableTask(TaskFunction func, void* user_data);

        /// Create a new task which couldn't be controlled anymore
        void SpawnDetachedTask(TaskFunction func, void* user_data);

    private:
        ftl::unique_ptr<TaskManagerImpl> m_Impl;
    };
}