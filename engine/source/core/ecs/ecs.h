#pragma once

#include "ftl/bitset.h"
#include "ftl/concepts.h"
#include "ftl/limits.h"
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

    // Base struct for all components, non-virtual
    // Derived classes are required to be trivially copy-able and movable!
    struct Component
    {

    };

    // Actual data of multiple components, type-erased and stored in contiguous array
    struct ComponentPool
    {
        uint8_t* data = nullptr;
        uint32_t element_size = 0;
        uint32_t element_count = 0;

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
    concept ComponentDerived = ftl::derived_from<T, Component>;

    template<ComponentDerived T>
    ComponentId GetComponentId() { return INVALID_COMPONENT_ID; }

    class CORE_API Manager
    {
    public:
        static Manager& Get();

        template<ComponentDerived T>
        bool HasComponent(EntityId entity) const { return HasComponent(entity, GetComponentId<T>()); }

        template<ComponentDerived T>
        const T* GetComponent(EntityId entity) const { return reinterpret_cast<const T*>(GetComponent(entity, GetComponentId<T>())); }

        template<ComponentDerived T>
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
