#pragma once

#include <vector>
#include <unordered_map>
#include <functional>
#include <typeindex>
#include <memory>

namespace GameLibrary
{
    class EventSystem;

    class SubscriptionToken
    {
    public:
        SubscriptionToken() = default;
        SubscriptionToken(EventSystem* system, std::type_index type, size_t id);
        ~SubscriptionToken();

        SubscriptionToken(const SubscriptionToken&) = delete;
        SubscriptionToken& operator=(const SubscriptionToken&) = delete;
        SubscriptionToken(SubscriptionToken&& other) noexcept;
        SubscriptionToken& operator=(SubscriptionToken&& other) noexcept;

    private:
        EventSystem* m_system = nullptr;
        std::type_index m_type = std::type_index(typeid(void));
        size_t m_id = 0;
        bool m_valid = false;
    };

    class EventSystem
    {
    public:
        template<typename T>
        [[nodiscard]] SubscriptionToken Subscribe(std::function<void(const T&)> callback)
        {
            auto key = std::type_index(typeid(T));
            size_t id = m_nextId++;

            m_callbacks[key].push_back({
                id,
                [callback](const void* event)
                {
                    callback(*static_cast<const T*>(event));
                }
                });

            return SubscriptionToken(this, key, id);
        }

        template<typename T>
        void Publish(const T& event)
        {
            auto key = std::type_index(typeid(T));
            auto it = m_callbacks.find(key);
            if (it == m_callbacks.end())
            {
                return;
            }

            for (auto& entry : it->second)
            {
                entry.callback(&event);
            }
        }

        void Unsubscribe(std::type_index type, size_t id);
        void Clear() noexcept;

    private:
        struct CallbackEntry
        {
            size_t id;
            std::function<void(const void*)> callback;
        };

        std::unordered_map<std::type_index, std::vector<CallbackEntry>> m_callbacks;
        size_t m_nextId = 0;
    };
}