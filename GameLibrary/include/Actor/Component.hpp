#pragma once

namespace GameLibrary
{
    class IGraphics;
    class Actor;

    class Component
    {
    public:
        explicit Component(Actor* owner) : m_owner(owner) {}
        virtual ~Component() = default;

        Component(const Component&) = delete;
        Component(Component&&) = delete;
        Component& operator=(const Component&) = delete;
        Component& operator=(Component&&) = delete;

        virtual void Init() {}
        virtual void Destroy() {}
        virtual void Update([[maybe_unused]] float deltaTime) {}
        virtual void Render([[maybe_unused]] IGraphics& graphics) {}

        [[nodiscard]] Actor* GetOwner() const noexcept { return m_owner; }

    protected:
        Actor* const m_owner{};
    };
} // namespace GameLibrary