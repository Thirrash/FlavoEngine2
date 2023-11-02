#include "core/ftl/soa.h"
#include "ecs/ecs.h"
#include "game_core/game_core.h"
#include "logger/logger.h"

int main()
{
    flavo::logger::info("Warthog project application starting...");
    flavo::game::test();

    enum SoaTypes : uint32_t
    {
        Distance,
        Count,
        Sign
    };

    flavo::ftl::soa<float, int, char> t;
    t.resize(12);
    auto dist_span = t.get_array<SoaTypes::Distance>();

    flavo::logger::info("Total sizeof: {}", t.allocated_size());
    flavo::logger::info("Distance count: {}", dist_span.size());

    // ------

    flavo::ecs::Level level;

    return 0;
}
