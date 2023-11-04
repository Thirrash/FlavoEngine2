#pragma once

#include "core/core_api.h"
#include "core/ftl/string.h"
#include "core/ftl/thread.h"
#include "core/ftl/unordered_map.h"

namespace flavo::parallel
{
    class CORE_API ThreadNameManager
    {
    public:
        static ThreadNameManager& Instance();

        void RegisterThread(ftl::thread::id id, ftl::string_view name);
        void UnregisterThread(ftl::thread::id id);

        ftl::string_view GetName(ftl::thread::id id) const;

    private:
        ftl::unordered_map<ftl::thread::id, ftl::string> m_Names;
    };
}
