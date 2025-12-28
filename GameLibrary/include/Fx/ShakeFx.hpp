#pragma once

#include "Fx.hpp"
#include "Interfaces/IGraphics.hpp"

namespace GameLibrary
{
    class ShakeFx : public Fx
    {
    public:
        ShakeFx(float intensity, float duration);

        void Update(float deltaTime) override;
        void Render([[maybe_unused]] IGraphics& graphics) override;

        [[nodiscard]] int32_t GetOffsetX() const noexcept { return m_offsetX; }
        [[nodiscard]] int32_t GetOffsetY() const noexcept { return m_offsetY; }

    private:
        float m_intensity{};
        float m_duration{};
        float m_elapsed{};

        int32_t m_offsetX{};
        int32_t m_offsetY{};
    };
} // namespace GameLibrary