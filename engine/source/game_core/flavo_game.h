#pragma once
#include "game_core_api.h"
#include "core/parallel/task.h"

struct HINSTANCE__;

namespace flavo::game
{
	class GAME_CORE_API FlavoGame
	{
	public:
		FlavoGame(HINSTANCE__* instance, bool show_cmd);

		parallel::Future<int> Loop();

	private:
		parallel::Future<void> UpdateGame();
		parallel::Future<void> SyncGameRender();
	};
}
