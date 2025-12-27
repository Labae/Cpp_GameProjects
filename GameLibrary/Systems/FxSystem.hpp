#pragma once

#include <vector>
#include <memory>
#include "FX/Fx.hpp"

namespace GameLibrary
{
    class Fx;
    class IGraphics;

    class FxSystem
    {
    public:
        FxSystem() = default;
        ~FxSystem() = default;

        template<typename T, typename... Args>
        void Spawn(Args&&... args)
        {
            m_effects.push_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

        void Update(float deltaTime);
        void Render(IGraphics& graphics);
        void Clear();

        [[nodiscard]] std::pair<int32_t, int32_t> GetShakeOffset() const noexcept;

    private:
        std::vector<std::unique_ptr<Fx>> m_effects;
    };
}