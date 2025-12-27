#pragma once

namespace GameLibrary
{
    class IGraphics;

    class Fx
    {
    public:
        virtual ~Fx() = default;

        virtual void Update(float deltaTime) = 0;
        virtual void Render(IGraphics& graphics) = 0;

        [[nodiscard]] bool IsFinished() const noexcept { return m_finished; }

    protected:
        bool m_finished{};
    };
}