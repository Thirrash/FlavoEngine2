#include "game_core/game_core.h"
#include "logger/logger.h"

int main()
{
    flavo::logger::info("Warthog project application starting...");
    flavo::game::test();

    return 0;
}
