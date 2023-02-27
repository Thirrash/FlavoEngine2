#pragma once

#include "ftl/bitset.h"
#include "ftl/concepts.h"
#include "ftl/limits.h"
#include "ftl/memory.h"
#include "ftl/unordered_map.h"
#include "ftl/vector.h"

namespace flavo::ecs
{
    // Entity handle
    using EntityId = uint32_t;
    constexpr EntityId INVALID_ENTITY_ID = ftl::numeric_limits<EntityId>::max();

    constexpr uint32_t MAX_NUM_COMPONENTS = 128;
    using ComponentMask = ftl::bitset<MAX_NUM_COMPONENTS>;

    using ComponentId = uint32_t;
    constexpr ComponentId INVALID_COMPONENT_ID = ftl::numeric_limits<ComponentId>::max();

    struct TestComponentDefault
    {
        uint64_t u;
        float f;
    };

    struct TestComponentCustom
    {
        ftl::unique_ptr<uint64_t> p;

        TestComponentCustom() = default;
        TestComponentCustom(const TestComponentCustom& src);
        TestComponentCustom(TestComponentCustom&& src);
        ~TestComponentCustom() = default;
        TestComponentCustom& operator=(TestComponentCustom src);
        friend void swap(TestComponentCustom& lhs, TestComponentCustom& rhs);
    };

    // Actual data of multiple components, type-erased and stored in contiguous array
    struct ComponentPool
    {
        uint8_t* data = nullptr;
        uint32_t element_count = 0;
        ComponentId id = INVALID_COMPONENT_ID;

        ComponentPool() = default;
        ComponentPool(const ComponentPool& src);
        ComponentPool(ComponentPool&& src) noexcept;
        ComponentPool& operator=(ComponentPool src);
        ~ComponentPool();

        friend void swap(ComponentPool& first, ComponentPool& second);
    };

    using ArchetypeId = uint64_t;
    struct Archetype
    {
        // Unique identifier of archetype, only used to speed up archetype lookup from component, it's value is meaningless
        ArchetypeId id;
        // Bit mask, where N-th set bit means that component with [ComponentId == N] is present in the archetype
        ComponentMask mask;
        // Pools for each component - to retrieve data for component with ComponentId, use: `Manager::m_Archetypes`
        // Each component pool should have the same `element_count` which is equal to the number of entities sharing archetype
        // Size: equal to number of set bits in `mask`
        ftl::vector<ComponentPool> component_pools;
    };

    struct EntityArchetypeRecord
    {
        // Non-owning reference to archetype
        Archetype* archetype = nullptr;
        // Entity index in component pool
        uint32_t component_pool_element = 0u;
    };

    struct ComponentArchetypeRecord
    {
        // Index on which the component is stored in `Archetype::component_pools`, per Archetype
        ftl::unordered_map<ArchetypeId, uint32_t> component_pool_index;
    };

    template<typename T>
    concept ComponentType = ftl::copyable<T> && ftl::movable<T>;

    static_assert(ComponentType<TestComponentDefault>);
    static_assert(ComponentType<TestComponentCustom>);

    /// Functions to specialize per each component type
    template<ComponentType T>
    ComponentId GetComponentId() { abort(); return INVALID_COMPONENT_ID; }

    template<> ComponentId GetComponentId<TestComponentDefault>() { return 0; }
    template<> ComponentId GetComponentId<TestComponentCustom>() { return 1; }

    uint32_t GetComponentSizeOf(ComponentId id);

    using CopyCtorFn = void(const uint8_t*, uint8_t*);
    void GetComponentCopyCtor(ComponentId id, CopyCtorFn*& fn);
    using MoveCtorFn = void(uint8_t*, uint8_t*);
    void GetComponentMoveCtor(ComponentId id, MoveCtorFn*& fn);

    /// Central point of access to all ECS-related structures and functions
    class CORE_API Manager
    {
    public:
        static Manager& Get();

        template<ComponentType T>
        bool HasComponent(EntityId entity) const { return HasComponent(entity, GetComponentId<T>()); }

        template<ComponentType T>
        const T* GetComponent(EntityId entity) const { return reinterpret_cast<const T*>(GetComponent(entity, GetComponentId<T>())); }

        template<ComponentType T>
        T* GetComponent(EntityId entity) { return reinterpret_cast<T*>(GetComponent(entity, GetComponentId<T>())); }

    private:
        bool HasComponent(EntityId entity, ComponentId component) const;
        const uint8_t* GetComponent(EntityId entity, ComponentId component) const;
        uint8_t* GetComponent(EntityId entity, ComponentId component);

    private:
        ftl::unordered_map<ComponentMask, Archetype> m_Archetypes;
        ftl::unordered_map<ComponentId, ComponentArchetypeRecord> m_ComponentArchetype;
        ftl::unordered_map<EntityId, EntityArchetypeRecord> m_EntityArchetype;

        // Next free id of component, 
        ComponentId m_NextComponentId;
    };
}
