#pragma once

#include "Interfaces/IInitializable.hpp"

#include <concepts>
#include <functional>
#include <memory>
#include <typeindex>
#include <unordered_map>
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
        template <typename TInterface, typename TImpl>
        void Register(ServiceLifetime lifetime = ServiceLifetime::Singleton)
        {
            auto key = std::type_index(typeid(TInterface));
            auto creator = []() -> std::shared_ptr<void>
            {
                return std::make_shared<TImpl>();
            };

            std::function<void(void*, ServiceContainer&)> initializer{};
            if constexpr (std::derived_from<TImpl, IInitializable>)
            {
                initializer = [](void* service, ServiceContainer& container)
                {
                    static_cast<TImpl*>(service)->Init(container);
                };
            }

            m_descriptors[key] = {creator, lifetime, initializer};
        }

        template <typename TInterface> void RegisterInstance(std::shared_ptr<TInterface> instance)
        {
            auto key = std::type_index(typeid(TInterface));
            m_singletonInstances[key] = instance;
        }

        template <typename TInterface> [[nodiscard]] TInterface* Resolve()
        {
            auto key = std::type_index(typeid(TInterface));

            auto instanceIt = m_singletonInstances.find(key);
            if (instanceIt != m_singletonInstances.end())
            {
                return static_cast<TInterface*>(instanceIt->second.get());
            }

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
                return ResolveTransient<TInterface>(descriptor.creator);
            }

            return nullptr;
        }

        void TryInit(std::type_index key, void* service)
        {
            auto it = m_descriptors.find(key);
            if (it != m_descriptors.end() && it->second.initializer)
            {
                it->second.initializer(service, *this);
            }
        }

        void BeginScope() noexcept { m_scopedInstances.clear(); }

    private:
        using Creator = std::function<std::shared_ptr<void>()>;

        struct ServiceDescriptor
        {
            Creator creator{};
            ServiceLifetime lifetime{};
            std::function<void(void*, ServiceContainer&)> initializer{};
        };

        template <typename TInterface>
        [[nodiscard]] TInterface* ResolveSingleton(std::type_index key, const Creator& creator)
        {
            auto it = m_singletonInstances.find(key);
            if (it == m_singletonInstances.end())
            {
                m_singletonInstances[key] = creator();
            }
            return static_cast<TInterface*>(m_singletonInstances[key].get());
        }

        template <typename TInterface>
        [[nodiscard]] TInterface* ResolveScoped(std::type_index key, const Creator& creator)
        {
            auto it = m_scopedInstances.find(key);
            if (it == m_scopedInstances.end())
            {
                m_scopedInstances[key] = creator();
            }
            return static_cast<TInterface*>(m_scopedInstances[key].get());
        }

        template <typename TInterface> [[nodiscard]] TInterface* ResolveTransient(const Creator& creator)
        {
            m_transientInstances.emplace_back(creator());
            return static_cast<TInterface*>(m_transientInstances.back().get());
        }

        std::unordered_map<std::type_index, ServiceDescriptor> m_descriptors{};
        std::unordered_map<std::type_index, std::shared_ptr<void>> m_singletonInstances{};
        std::unordered_map<std::type_index, std::shared_ptr<void>> m_scopedInstances{};
        std::vector<std::shared_ptr<void>> m_transientInstances{};
    };
} // namespace GameLibrary