#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <functional>
#include <vector>

namespace GameLibrary
{
    enum class ServiceLifetime
    {
        Singleton,
        Scoped,
        Transient
    };

    class ServiceContainer
    {
    public:
        template<typename TInterface, typename TImpl>
        void Register(ServiceLifetime lifetime = ServiceLifetime::Singleton)
        {
            auto key = std::type_index(typeid(TInterface));
            auto creator = []() -> std::shared_ptr<void>
                {
                    return std::make_shared<TImpl>();
                };

            m_descriptors[key] = { creator, lifetime };
        }

        template<typename TInterface>
        [[nodiscard]] TInterface* Resolve()
        {
            auto key = std::type_index(typeid(TInterface));
            auto it = m_descriptors.find(key);
            if (it == m_descriptors.end())
            {
                return nullptr;
            }

            auto& descriptor = it->second;

            switch (descriptor.lifetime)
            {
            case ServiceLifetime::Singleton:
                return ResolveSingleton<TInterface>(key, descriptor.creator);
            case ServiceLifetime::Scoped:
                return ResolveScoped<TInterface>(key, descriptor.creator);
            case ServiceLifetime::Transient:
                return ResolveTransient<TInterface>(key, descriptor.creator);
            }

            return nullptr;
        }

        void BeginScope() noexcept
        {
            m_scopedInstances.clear();
        }

    private:
        using Creator = std::function<std::shared_ptr<void>()>;

        struct ServiceDescriptor
        {
            Creator creator;
            ServiceLifetime lifetime;
        };

        template<typename TInterface>
        [[nodiscard]] TInterface* ResolveSingleton(std::type_index key, const Creator& creator)
        {
            auto it = m_singletonInstances.find(key);
            if (it == m_singletonInstances.end())
            {
                m_singletonInstances[key] = creator();
            }
            return static_cast<TInterface*>(m_singletonInstances[key].get());
        }

        template<typename TInterface>
        [[nodiscard]] TInterface* ResolveScoped(std::type_index key, const Creator& creator)
        {
            auto it = m_scopedInstances.find(key);
            if (it == m_scopedInstances.end())
            {
                m_scopedInstances[key] = creator();
            }
            return static_cast<TInterface*>(m_scopedInstances[key].get());
        }

        template<typename TInterface>
        [[nodiscard]] TInterface* ResolveTransient(std::type_index key, const Creator& creator)
        {
            m_transientInstances.push_back(creator());
            return static_cast<TInterface*>(m_transientInstances.back().get());
        }

        std::unordered_map<std::type_index, ServiceDescriptor> m_descriptors;
        std::unordered_map<std::type_index, std::shared_ptr<void>> m_singletonInstances;
        std::unordered_map<std::type_index, std::shared_ptr<void>> m_scopedInstances;
        std::vector<std::shared_ptr<void>> m_transientInstances;
    };
}