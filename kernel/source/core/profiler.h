#pragma once

#include "optick/src/optick.h"

#define FLAVO_PROFILE_THREAD(thread_name) OPTICK_THREAD(thread_name)
#define FLAVO_PROFILE_START_THREAD(thread_name) OPTICK_START_THREAD(thread_name)
#define FLAVO_PROFILE_END_THREAD() OPTICK_STOP_THREAD()
#define FLAVO_PROFILE_EVENT(event_name) OPTICK_EVENT(event_name)
#define FLAVO_PROFILE_FRAME(frame_name) OPTICK_FRAME(frame_name)
