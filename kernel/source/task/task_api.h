#pragma once

#ifdef TASK_API_EXPORT
#define TASK_API __declspec(dllexport)
#else
#define TASK_API __declspec(dllimport)
#endif
