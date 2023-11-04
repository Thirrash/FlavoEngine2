#include "task_manager.h"

#include "core/ftl/format.h"
#include "core/ftl/thread.h"
#include "core/ftl/vector.h"
#include "core/parallel/thread_name.h"

#include "thread_pool/BS_thread_pool.hpp"

namespace flavo::parallel
{
    class TaskManagerImpl
    {
    public:
        TaskManagerImpl();

        ftl::future<void> SpawnWaitableTask(TaskFunction func, void* user_data);
        void SpawnDetachedTask(TaskFunction func, void* user_data);

    private:
        BS::thread_pool m_Pool;
    };

    TaskManagerImpl::TaskManagerImpl()
    {
        // Attach names to worker threads
        ftl::vector<ftl::thread::id> ids = m_Pool.get_thread_ids();

        uint32_t idx = 0;
        for (ftl::thread::id id : ids)
        {
            ftl::string name = ftl::format("Worker {}", idx);
            ThreadNameManager::Instance().RegisterThread(id, name);
            ++idx;
        }
    }

    ftl::future<void> TaskManagerImpl::SpawnWaitableTask(TaskFunction func, void* user_data)
    {
        TaskParams params;
        params.user_data = user_data;

        return m_Pool.submit(func, params);
    }

    void TaskManagerImpl::SpawnDetachedTask(TaskFunction func, void* user_data)
    {
        TaskParams params;
        params.user_data = user_data;

        m_Pool.push_task(func, params);
    }

    TaskManager& TaskManager::Instance()
    {
        static TaskManager mgr;
        return mgr;
    }

    TaskManager::TaskManager()
        : m_Impl(ftl::make_unique<TaskManagerImpl>())
    {

    }

    TaskManager::~TaskManager()
    {

    }

    ftl::future<void> TaskManager::SpawnWaitableTask(TaskFunction func, void* user_data)
    {
        return m_Impl->SpawnWaitableTask(func, user_data);
    }

    void TaskManager::SpawnDetachedTask(TaskFunction func, void* user_data)
    {
        m_Impl->SpawnDetachedTask(func, user_data);
    }
}
