#pragma once

#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

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
        EventSystem* m_system{};
        std::type_index m_type{typeid(void)};
        size_t m_id{};
        bool m_valid{};
    };

    class EventSystem
    {
    public:
        EventSystem() = default;
        ~EventSystem() = default;

        EventSystem(const EventSystem&) = delete;
        EventSystem(EventSystem&&) = delete;
        EventSystem& operator=(const EventSystem&) = delete;
        EventSystem& operator=(EventSystem&&) = delete;

        template <typename T> [[nodiscard]] SubscriptionToken Subscribe(std::function<void(const T&)> callback)
        {
            auto key = std::type_index(typeid(T));
            size_t id = m_nextId++;

            m_callbacks[key].push_back({id, [callback](const void* event)
                                        {
                                            callback(*static_cast<const T*>(event));
                                        }});

            return SubscriptionToken(this, key, id);
        }

        template <typename T> void Publish(const T& event)
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
            size_t id{};
            std::function<void(const void*)> callback{};
        };

        std::unordered_map<std::type_index, std::vector<CallbackEntry>> m_callbacks{};
        size_t m_nextId{};
    };
} // namespace GameLibrary