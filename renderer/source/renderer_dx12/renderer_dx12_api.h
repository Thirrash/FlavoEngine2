#pragma once

#ifdef RENDERER_DX12_API_EXPORT
#define RENDERER_DX12_API __declspec(dllexport)
#else
#define RENDERER_DX12_API __declspec(dllimport)
#endif
