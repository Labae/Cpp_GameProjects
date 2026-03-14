#ifndef OBJECTPOOL_HPP
#define OBJECTPOOL_HPP

#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

namespace GameLibrary
{
    template <typename T>
    class ObjectPool
    {
    public:
        explicit ObjectPool(int32_t initialSize, std::function<std::unique_ptr<T>()> factory)
            : m_factory(std::move(factory))
        {
            for (int32_t i = 0; i < initialSize; ++i)
            {
                m_available.emplace_back(m_factory());
            }
        }

        ~ObjectPool() = default;

        ObjectPool(const ObjectPool&) = delete;
        ObjectPool(ObjectPool&&) = default;
        ObjectPool& operator=(const ObjectPool&) = delete;
        ObjectPool& operator=(ObjectPool&&) = default;

        [[nodiscard]] T* Acquire()
        {
            if (m_available.empty())
            {
                m_available.emplace_back(m_factory());
            }

            auto obj = std::move(m_available.back());
            m_available.pop_back();

            auto* ptr = obj.get();
            m_inUse.emplace_back(std::move(obj));

            return ptr;
        }

        void Release(T* object)
        {
            assert(object && "Cannot release null object");

            const auto it = std::find_if(m_inUse.begin(), m_inUse.end(),
                [object](const auto& ptr) { return ptr.get() == object; });

            if (it == m_inUse.end())
            {
                return;
            }

            m_available.emplace_back(std::move(*it));
            m_inUse.erase(it);
        }

        void ReleaseAll()
        {
            for (auto& obj : m_inUse)
            {
                m_available.emplace_back(std::move(obj));
            }
            m_inUse.clear();
        }

        [[nodiscard]] int32_t GetAvailableCount() const noexcept
        {
            return static_cast<int32_t>(m_available.size());
        }

        [[nodiscard]] int32_t GetInUseCount() const noexcept
        {
            return static_cast<int32_t>(m_inUse.size());
        }

        [[nodiscard]] int32_t GetTotalCount() const noexcept
        {
            return GetAvailableCount() + GetInUseCount();
        }

    private:
        std::function<std::unique_ptr<T>()> m_factory;
        std::vector<std::unique_ptr<T>> m_available{};
        std::vector<std::unique_ptr<T>> m_inUse{};
    };
} // namespace GameLibrary

#endif // OBJECTPOOL_HPP