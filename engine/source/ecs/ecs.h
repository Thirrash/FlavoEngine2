#pragma once
#include "ecs_api.h"

#include "flecs/flecs.h"

namespace flavo::ecs
{
    class ECS_API Level
    {
    public:
        Level();

    private:
        flecs::world m_World;
    };
}
