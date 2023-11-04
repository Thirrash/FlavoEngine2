#include "core/ftl/chrono.h"
#include "core/ftl/soa.h"
#include "core/logger/logger.h"
#include "core/parallel/task_manager.h"
#include "core/parallel/thread_name.h"
#include "ecs/ecs.h"
#include "game_core/game_core.h"

struct TaskData
{
    uint32_t input;
    uint32_t output;
};

void ThreadTask(flavo::parallel::TaskParams params)
{
    TaskData* user_data = static_cast<TaskData*>(params.user_data);
    flavo::logger::info("Task in progress. Input: {}", user_data->input);
    flavo::ftl::this_thread::sleep_for(flavo::ftl::chrono::seconds() * 3);
    user_data->output = user_data->input + 100;
}

int main()
{
    flavo::parallel::ThreadNameManager::Instance().RegisterThread(flavo::ftl::this_thread::get_id(), "Main Thread");

    flavo::logger::info("Warthog project application starting...");
    flavo::game::test();

    enum SoaTypes : uint32_t
    {
        Distance,
        Count,
        Sign
    };

    flavo::ftl::soa<float, int, char> t;
    t.resize(12);
    auto dist_span = t.get_array<SoaTypes::Distance>();

    flavo::logger::info("Total sizeof: {}", t.allocated_size());
    flavo::logger::info("Distance count: {}", dist_span.size());

    // ------

    flavo::ecs::Level level;

    // ------
    TaskData task_data1, task_data2;
    task_data1.input = 10;
    task_data2.input = 20;

    flavo::ftl::future<void> task_future1 = flavo::parallel::TaskManager::Instance().SpawnWaitableTask(ThreadTask, &task_data1);
    flavo::ftl::future<void> task_future2 = flavo::parallel::TaskManager::Instance().SpawnWaitableTask(ThreadTask, &task_data2);
    task_future1.wait();
    task_future2.wait();

    flavo::logger::info("Task1: {}, Task2: {}", task_data1.output, task_data2.output);

    return 0;
}
