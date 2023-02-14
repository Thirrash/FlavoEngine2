#include "ecs/ecs.h"

#include "ftl/utility.h"

namespace flavo::ecs
{
    ComponentPool::ComponentPool(const ComponentPool& src)
        : data(nullptr)
        , element_count(src.element_count)
        , element_size(src.element_size)
    {
        if (src.element_count > 0)
        {
            // Assumption: trivially copy-able
            const uint32_t num_bytes = src.element_size * src.element_count;
            data = new uint8_t[num_bytes];
            memcpy(data, src.data, num_bytes);
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
        ftl::swap(first.element_size, second.element_size);
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
