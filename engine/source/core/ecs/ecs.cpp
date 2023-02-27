#include "ecs/ecs.h"

#include "ftl/array.h"
#include "ftl/utility.h"

namespace flavo::ecs
{
    TestComponentCustom::TestComponentCustom(const TestComponentCustom& src)
        : p(src.p ? ftl::make_unique<uint64_t>(*src.p) : nullptr)
    {
    }

    TestComponentCustom::TestComponentCustom(TestComponentCustom&& src)
        : TestComponentCustom()
    {
        swap(*this, src);
    }

    TestComponentCustom& TestComponentCustom::operator=(TestComponentCustom src)
    {
        swap(*this, src);
        return *this;
    }

    void swap(TestComponentCustom& lhs, TestComponentCustom& rhs)
    {
        ftl::swap(lhs.p, rhs.p);
    }

    ComponentPool::ComponentPool(const ComponentPool& src)
        : data(nullptr)
        , element_count(src.element_count)
        , id(src.id)
    {
        if (src.element_count > 0)
        {
            const uint32_t element_size = GetComponentSizeOf(src.id);
            const uint32_t num_bytes = element_size * src.element_count;
            CopyCtorFn* copy_ctor = nullptr;
            GetComponentCopyCtor(src.id, copy_ctor);

            data = new uint8_t[num_bytes];
            for (uint32_t i = 0; i < src.element_count; ++i)
            {
                const uint64_t ptr_offset = element_size * i;
                copy_ctor(src.data + ptr_offset, data + ptr_offset);
            }
        }
    }

    ComponentPool::ComponentPool(ComponentPool&& src) noexcept
        : ComponentPool()
    {
        swap(*this, src);
    }

    ComponentPool& ComponentPool::operator=(ComponentPool src)
    {
        swap(*this, src);
        return *this;
    }

    ComponentPool::~ComponentPool()
    {
        delete[] data;
    }

    void swap(ComponentPool& first, ComponentPool& second)
    {
        ftl::swap(first.data, second.data);
        ftl::swap(first.element_count, second.element_count);
        ftl::swap(first.id, second.id);
    }

    uint32_t GetComponentSizeOf(ComponentId id)
    {
        // #TODO Generate via header tool
        static constexpr uint32_t DATA_COUNT = 2;
        static constexpr ftl::array<uint32_t, DATA_COUNT> DATA =
        {
            sizeof(TestComponentDefault),
            sizeof(TestComponentCustom),
        };

        if (id >= DATA_COUNT)
            abort();

        return DATA[id];
    }

    void GetComponentCopyCtor(ComponentId id, CopyCtorFn*& fn)
    {
        // #TODO Generate via header tool
        static constexpr uint32_t DATA_COUNT = 2;
        static constexpr ftl::array<CopyCtorFn*, DATA_COUNT> DATA =
        {
            [](const uint8_t* src, uint8_t* dst) constexpr { new (dst) TestComponentDefault(*reinterpret_cast<const TestComponentDefault*>(src)); },
            [](const uint8_t* src, uint8_t* dst) constexpr { new (dst) TestComponentCustom(*reinterpret_cast<const TestComponentCustom*>(src)); },
        };

        if (id >= DATA_COUNT)
            abort();

        fn = DATA[id];
    }

    void GetComponentMoveCtor(ComponentId id, MoveCtorFn*& fn)
    {
        // #TODO Generate via header tool
        static constexpr uint32_t DATA_COUNT = 2;
        static constexpr ftl::array<MoveCtorFn*, DATA_COUNT> DATA =
        {
            [](uint8_t* src, uint8_t* dst) constexpr { new (dst) TestComponentDefault(ftl::move(*reinterpret_cast<TestComponentDefault*>(src))); },
            [](uint8_t* src, uint8_t* dst) constexpr { new (dst) TestComponentCustom(ftl::move(*reinterpret_cast<TestComponentCustom*>(src))); },
        };

        if (id >= DATA_COUNT)
            abort();

        fn = DATA[id];
    }

    Manager& Manager::Get()
    {
        static Manager INSTANCE;
        return INSTANCE;
    }

    bool Manager::HasComponent(EntityId entity, ComponentId component) const
    {
        // Retrieve current archetype of the entity
        const auto entity_archetype_it = m_EntityArchetype.find(entity);
        if (entity_archetype_it == m_EntityArchetype.end())
        {
            // Entity is not registered in the Manager
            return false;
        }

        const EntityArchetypeRecord& entity_archetype_record = entity_archetype_it->second;
        const ArchetypeId& archetype_id = entity_archetype_record.archetype->id;

        // Retrieve index of the component in the found archetype's component pool
        const auto component_archetype_it = m_ComponentArchetype.find(component);
        if (component_archetype_it == m_ComponentArchetype.end())
        {
            // Component type is not registered in the Manager
            return false;
        }

        const ComponentArchetypeRecord& component_archetype_record = component_archetype_it->second;
        const auto component_pool_it = component_archetype_record.component_pool_index.find(archetype_id);
        if (component_pool_it == component_archetype_record.component_pool_index.end())
        {
            // Component is not present in the archetype of the entity
            return false;
        }

        // The entity has the component
        return true;
    }
}
