#pragma once
#include "game_core_api.h"

struct HINSTANCE__;

namespace flavo::game
{
	class GAME_CORE_API FlavoGame
	{
	public:
		FlavoGame(HINSTANCE__* instance, bool show_cmd);

		int Loop();

	private:
		void UpdateGame();
		void SyncGameRender();
	};
}
