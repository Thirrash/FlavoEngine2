#include "core/ftl/chrono.h"
#include "core/ftl/soa.h"
#include "core/logger/logger.h"
#include "core/parallel/task_manager.h"
#include "core/parallel/thread_name.h"
#include "ecs/ecs.h"
#include "game_core/flavo_game.h"
#include "renderer/render_manager.h"
#include "task/task.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

void ThreadStartedCallback(std::string_view thread_name)
{
    flavo::parallel::ThreadNameManager::Instance().RegisterThread(flavo::ftl::this_thread::get_id(), thread_name);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmdline, int show_cmd)
{
    flavo::parallel::ThreadNameManager::Instance().RegisterThread(flavo::ftl::this_thread::get_id(), "Main Thread");

    flavo::logger::Info("Warthog project application starting...");

    flavo::task::RuntimeOptions options;
    options.thread_started_callback = ThreadStartedCallback;
    flavo::task::Runtime task_runtime(options);
    flavo::task::SetRuntime(task_runtime);


    flavo::game::FlavoGame game_instance(instance, show_cmd);
    return game_instance.Loop().get();
}
