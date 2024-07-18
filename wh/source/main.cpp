#include "core/ftl/chrono.h"
#include "core/ftl/soa.h"
#include "core/logger/logger.h"
#include "core/parallel/task.h"
#include "core/parallel/thread_name.h"
#include "core/platform/os_windows.h"
#include "core/profiler.h"
#include "ecs/ecs.h"
#include "game_core/flavo_game.h"
#include "renderer/render_manager.h"

static constexpr const char* FLAVO_MAIN_THREAD_NAME = "Main Thread";

void ThreadStartedCallback(std::string_view thread_name)
{
    FLAVO_PROFILE_START_THREAD(thread_name.data());
    flavo::parallel::ThreadNameManager::Instance().RegisterThread(flavo::ftl::this_thread::get_id(), thread_name);
}

void ThreadEndedCallback(std::string_view thread_name)
{
	flavo::parallel::ThreadNameManager::Instance().UnregisterThread(flavo::ftl::this_thread::get_id());
    FLAVO_PROFILE_END_THREAD();
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmdline, int show_cmd)
{
    FLAVO_PROFILE_THREAD(FLAVO_MAIN_THREAD_NAME);
    flavo::parallel::ThreadNameManager::Instance().RegisterThread(flavo::ftl::this_thread::get_id(), FLAVO_MAIN_THREAD_NAME);

    FLAVO_PROFILE_EVENT(__FUNCTION__);

    flavo::logger::Info("Warthog project application starting...");

    flavo::parallel::RuntimeOptions options;
    options.thread_started_callback = ThreadStartedCallback;
    options.thread_terminated_callback = ThreadEndedCallback;
    flavo::parallel::Runtime task_runtime(options);
    flavo::parallel::SetRuntime(task_runtime);

    flavo::game::FlavoGame game_instance(instance, show_cmd);
    return game_instance.Loop().get();
}
