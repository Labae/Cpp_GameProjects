#pragma once

#include "Fx/Fx.hpp"

#include <memory>
#include <vector>

namespace GameLibrary
{
    class IGraphics;

    class FxSystem
    {
    public:
        FxSystem() = default;
        ~FxSystem() = default;

        FxSystem(const FxSystem&) = delete;
        FxSystem(FxSystem&&) = delete;
        FxSystem& operator=(const FxSystem&) = delete;
        FxSystem& operator=(FxSystem&&) = delete;

        template <typename T, typename... Args> void Spawn(Args&&... args)
        {
            m_effects.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

        void Update(float deltaTime);
        void Render(IGraphics& graphics);
        void Clear();

        [[nodiscard]] std::pair<int32_t, int32_t> GetShakeOffset() const noexcept;

    private:
        std::vector<std::unique_ptr<Fx>> m_effects{};
    };
} // namespace GameLibrary