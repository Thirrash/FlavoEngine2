#pragma once

#ifdef GAME_CORE_API_EXPORT
#define GAME_CORE_API __declspec(dllexport)
#else
#define GAME_CORE_API __declspec(dllimport)
#endif
