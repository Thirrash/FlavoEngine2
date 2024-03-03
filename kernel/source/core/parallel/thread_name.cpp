#include "thread_name.h"

namespace flavo::parallel
{
    ThreadNameManager& ThreadNameManager::Instance()
    {
        static ThreadNameManager mgr;
        return mgr;
    }

    void ThreadNameManager::RegisterThread(ftl::thread::id id, ftl::string_view name)
    {
        m_Names[id] = name;
    }

    void ThreadNameManager::UnregisterThread(ftl::thread::id id)
    {
        m_Names.erase(id);
    }

    ftl::string_view ThreadNameManager::GetName(ftl::thread::id id) const
    {
        auto&& it = m_Names.find(id);
        return (it != m_Names.end()) ? std::string_view(it->second) : std::string_view("<Unknown>");
    }
}
