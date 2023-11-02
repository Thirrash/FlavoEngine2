#pragma once

#ifdef LOGGER_API_EXPORT
#define LOGGER_API __declspec(dllexport)
#else
#define LOGGER_API __declspec(dllimport)
#endif
