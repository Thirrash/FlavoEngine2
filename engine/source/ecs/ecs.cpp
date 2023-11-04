#include "ecs.h"

#include "core/logger/logger.h"

#include "flecs/flecs.h"

namespace flavo::ecs
{
    class LevelImpl
    {
    public:
        LevelImpl();

        void TestPrint() const;

    private:
        flecs::world m_World;
    };

    LevelImpl::LevelImpl()
    {

    }

    void LevelImpl::TestPrint() const
    {
        flecs::entity root_entity = m_World.entity("Root");
        flavo::logger::info("Created root entity: {}", root_entity.name().c_str());
        root_entity.destruct();
    }

    Level::Level()
        : m_Impl(ftl::make_unique<LevelImpl>())
    {
        m_Impl->TestPrint();
    }

    Level::Level(Level&& src) noexcept
        : m_Impl(ftl::move(src.m_Impl))
    {
    }

    Level& Level::operator=(Level&& src) noexcept
    {
        if (this != &src)
        {
            ftl::swap(m_Impl, src.m_Impl);
        }

        return *this;
    }

    Level::~Level()
    {

    }
}