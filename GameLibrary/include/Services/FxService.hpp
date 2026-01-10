#pragma once

#include "Fx/Fx.hpp"

#include <memory>
#include <vector>

namespace GameLibrary
{
    class IGraphics;

    class FxService
    {
    public:
        FxService() = default;
        ~FxService() = default;

        FxService(const FxService&) = delete;
        FxService(FxService&&) = delete;
        FxService& operator=(const FxService&) = delete;
        FxService& operator=(FxService&&) = delete;

        template <typename T, typename... Args> void Spawn(Args&&... args)
        {
            m_effects.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

        void Update(float deltaTime);
        void Render(IGraphics& graphics) const;
        void Clear();

        [[nodiscard]] std::pair<int32_t, int32_t> GetShakeOffset() const noexcept;

    private:
        std::vector<std::unique_ptr<Fx>> m_effects{};
    };
} // namespace GameLibrary