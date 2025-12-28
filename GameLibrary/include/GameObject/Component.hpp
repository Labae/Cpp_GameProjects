#pragma once

namespace GameLibrary
{
    class IGraphics;

    class Component
    {
    public:
        Component() = default;
        virtual ~Component() = default;

        Component(const Component&) = delete;
        Component(Component&&) = delete;
        Component& operator=(const Component&) = delete;
        Component& operator=(Component&&) = delete;

        virtual void Init() {}
        virtual void Destroy() {}
        virtual void Update([[maybe_unused]] float deltaTime) {}
        virtual void Render([[maybe_unused]] IGraphics& graphics) {}
    };
} // namespace GameLibrary