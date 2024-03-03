#pragma once

#include "core/ftl/algorithm.h"
#include "core/ftl/array.h"
#include "core/ftl/numeric.h"
#include "core/ftl/span.h"
#include "core/ftl/tuple.h"
#include "core/ftl/utility.h"
#include "core/ftl/vector.h"
#include "core/utils/constexpr_loop.h"
#include "core/utils/raw_array_ops.h"

namespace flavo::ftl
{
    /// Structure of Arrays
    // Can hold multiple arrays of potentially different types but with the same element count
    // #TODO Handle alignment of passed types
    template<typename ...Types>
    class soa
    {
    public:
        template<uint64_t N>
        using value_type = typename ftl::tuple_element<N, ftl::tuple<Types...>>::type;

        static constexpr uint64_t NUM_TYPES = sizeof...(Types);
        static constexpr ftl::array<uint64_t, NUM_TYPES> TYPES_SIZEOF = { sizeof(Types)... };

        soa() = default;
        soa(const soa& src);
        soa(soa&& src) noexcept;
        ~soa();
        soa& operator=(soa src) noexcept;
        friend void swap(soa& lhs, soa& rhs);

        // Element count
        uint64_t size() const { return m_NumElements; }
        bool empty() const { return m_NumElements == 0; }
        // Allocated memory in bytes, doesn't include sizeof(this)
        uint64_t allocated_size() const { return calculate_allocated_bytes(m_NumElements); }

        // Clear and deallocate memory
        void destroy();
        // Resize container, destroying previous elements
        void resize(uint64_t num_elements);

        // Stored array retrieval functions
        template<uint64_t type_index>
        ftl::span<value_type<type_index>> get_array();
        template<uint64_t type_index>
        const ftl::span<value_type<type_index>> get_array() const;

    private:
        static uint64_t calculate_allocated_bytes(uint64_t num_elements);
        static uint64_t calculate_offset_bytes(uint64_t type_index, uint64_t element_index, uint64_t num_elements);

        uint8_t* m_Data = nullptr;
        uint64_t m_NumElements = 0;
    };

    template<typename ...Types>
    inline soa<Types...>::soa(const soa& src)
        : flavo::utils::CopyRawArray(src.m_Data, src.m_NumElements)
        , m_NumElements(src.m_NumElements)
    {
    }

    template<typename ...Types>
    inline soa<Types...>::soa(soa&& src) noexcept
        : soa()
    {
        swap(*this, src);
    }

    template<typename ...Types>
    inline soa<Types...>::~soa()
    {
        delete[] m_Data;
    }

    template<typename ...Types>
    inline soa<Types...>& soa<Types...>::operator=(soa src) noexcept
    {
        swap(*this, src);
        return *this;
    }

    template<typename ...Types>
    void swap(soa<Types...>& lhs, soa<Types...>& rhs)
    {
        swap(lhs.m_Data, rhs.m_Data);
        swap(lhs.m_NumElements, rhs.m_NumElements);
    }

    template<typename ...Types>
    inline void soa<Types...>::destroy()
    {
        delete[] m_Data;
        m_NumElements = 0;
    }

    template<typename ...Types>
    inline void soa<Types...>::resize(uint64_t num_elements)
    {
        destroy();

        if (num_elements > 0) [[likely]]
        {
            const uint64_t new_size_bytes = calculate_allocated_bytes(num_elements);
            m_Data = new uint8_t[new_size_bytes];
            m_NumElements = num_elements;
        }
    }

    template<typename ...Types>
    template<uint64_t type_index>
    inline ftl::span<typename soa<Types...>::value_type<type_index>> soa<Types...>::get_array()
    {
        if (!m_Data)
            return {};

        const uint64_t offset = calculate_offset_bytes(type_index, 0, size());
        auto* ptr = reinterpret_cast<value_type<type_index>*>(m_Data[offset]);
        return { ptr, size() };
    }

    template<typename ...Types>
    template<uint64_t type_index>
    inline const ftl::span<typename soa<Types...>::value_type<type_index>> soa<Types...>::get_array() const
    {
        if (!m_Data)
            return {};

        const uint64_t offset = calculate_offset_bytes(type_index, 0, size());
        auto* ptr = reinterpret_cast<const value_type<type_index>*>(m_Data[offset]);
        return { ptr, size() };
    }

    template<typename ...Types>
    inline uint64_t soa<Types...>::calculate_allocated_bytes(uint64_t num_elements)
    {
        return std::accumulate(TYPES_SIZEOF.begin(), TYPES_SIZEOF.end(), 0ui64, [&](uint64_t sum, uint64_t curr) { return sum + curr * num_elements; });
    }

    template<typename ...Types>
    uint64_t soa<Types...>::calculate_offset_bytes(uint64_t type_index, uint64_t element_index, uint64_t num_elements)
    {
        uint64_t offset = 0;
        for (uint64_t prev_type_index = 0; prev_type_index < type_index; ++prev_type_index)
        {
            offset += TYPES_SIZEOF[prev_type_index] * num_elements;
        }

        return offset + TYPES_SIZEOF[type_index] * element_index;
    }
}
