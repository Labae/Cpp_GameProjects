#pragma once

namespace GameLibrary
{
    class IGraphics;

    class Fx
    {
    public:
        Fx() = default;
        virtual ~Fx() = default;

        Fx(const Fx&) = delete;
        Fx(Fx&&) = delete;
        Fx& operator=(const Fx&) = delete;
        Fx& operator=(Fx&&) = delete;

        virtual void Update(float deltaTime) = 0;
        virtual void Render(IGraphics& graphics) = 0;

        [[nodiscard]] bool IsFinished() const noexcept { return m_finished; }

    protected:
        bool m_finished{};
    };
} // namespace GameLibrary