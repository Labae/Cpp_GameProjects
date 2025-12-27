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
        virtual void Update(float deltaTime) {}
        virtual void Render(IGraphics& graphics) {}
    };
}