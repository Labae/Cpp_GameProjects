#include "Services/EventService.hpp"

#include <algorithm>

namespace GameLibrary
{
    SubscriptionToken::SubscriptionToken(EventService* system, std::type_index type, size_t id)
        : m_system(system)
        , m_type(type)
        , m_id(id)
        , m_valid(true)
    {
    }

    SubscriptionToken::~SubscriptionToken()
    {
        if (m_valid && m_system)
        {
            m_system->Unsubscribe(m_type, m_id);
        }
    }

    SubscriptionToken::SubscriptionToken(SubscriptionToken&& other) noexcept
        : m_system(other.m_system)
        , m_type(other.m_type)
        , m_id(other.m_id)
        , m_valid(other.m_valid)
    {
        other.m_valid = false;
    }

    SubscriptionToken& SubscriptionToken::operator=(SubscriptionToken&& other) noexcept
    {
        if (this != &other)
        {
            if (m_valid && m_system)
            {
                m_system->Unsubscribe(m_type, m_id);
            }

            m_system = other.m_system;
            m_type = other.m_type;
            m_id = other.m_id;
            m_valid = other.m_valid;

            other.m_valid = false;
        }
        return *this;
    }

    void EventService::Unsubscribe(std::type_index type, size_t id)
    {
        auto it = m_callbacks.find(type);
        if (it == m_callbacks.end())
        {
            return;
        }

        auto& vec = it->second;
        vec.erase(std::remove_if(vec.begin(), vec.end(), [id](const CallbackEntry& entry) { return entry.id == id; }),
                  vec.end());
    }

    void EventService::Clear() noexcept
    {
        m_callbacks.clear();
    }
} // namespace GameLibrary