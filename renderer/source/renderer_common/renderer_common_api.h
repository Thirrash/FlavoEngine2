#pragma once

#ifdef RENDERER_COMMON_API_EXPORT
#define RENDERER_COMMON_API __declspec(dllexport)
#else
#define RENDERER_COMMON_API __declspec(dllimport)
#endif
