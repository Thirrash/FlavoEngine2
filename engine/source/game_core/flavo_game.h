#pragma once
#include "game_core_api.h"
#include "task/task.h"

struct HINSTANCE__;

namespace flavo::game
{
	class GAME_CORE_API FlavoGame
	{
	public:
		FlavoGame(HINSTANCE__* instance, bool show_cmd);

		task::Future<int> Loop();

	private:
		static task::Future<void> UpdateGame();
		void SyncGameRender();
	};
}
