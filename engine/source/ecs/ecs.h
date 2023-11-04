#pragma once
#include "ecs_api.h"

#include "core/ftl/memory.h"

namespace flavo::ecs
{
    class LevelImpl;

    class ECS_API Level
    {
    public:
        Level();
        Level(const Level&) = delete;
        Level(Level&& src) noexcept;
        Level& operator=(const Level&) = delete;
        Level& operator=(Level&& src) noexcept;
        ~Level();

    private:
        ftl::unique_ptr<LevelImpl> m_Impl;
    };
}
