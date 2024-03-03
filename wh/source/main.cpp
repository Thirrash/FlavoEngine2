#include "core/ftl/chrono.h"
#include "core/ftl/soa.h"
#include "core/logger/logger.h"
#include "core/parallel/task_manager.h"
#include "core/parallel/thread_name.h"
#include "ecs/ecs.h"
#include "game_core/flavo_game.h"
#include "renderer/render_manager.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

struct TaskData
{
    uint32_t input;
    uint32_t output;
};

void ThreadTask(flavo::parallel::TaskParams params)
{
    TaskData* user_data = static_cast<TaskData*>(params.user_data);
    flavo::logger::Info("Task in progress. Input: {}", user_data->input);
    flavo::ftl::this_thread::sleep_for(flavo::ftl::chrono::seconds() * 3);
    user_data->output = user_data->input + 100;
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmdline, int show_cmd)
{
    flavo::parallel::ThreadNameManager::Instance().RegisterThread(flavo::ftl::this_thread::get_id(), "Main Thread");

    flavo::logger::Info("Warthog project application starting...");

    flavo::game::FlavoGame game_instance(instance, show_cmd);
    return game_instance.Loop();
}
