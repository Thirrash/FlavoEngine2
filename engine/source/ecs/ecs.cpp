#include "ecs.h"

#include "logger/logger.h"

namespace flavo::ecs
{
    Level::Level()
    {
        flecs::entity root_entity = m_World.entity("Root");
        flavo::logger::info("Created root entity: {}", root_entity.name().c_str());
    }
}