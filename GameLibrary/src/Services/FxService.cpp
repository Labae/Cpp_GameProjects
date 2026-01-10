#include "../../include/Services/FxService.hpp"

#include "Fx/Fx.hpp"
#include "Fx/ShakeFx.hpp"

#include <algorithm>

namespace GameLibrary
{
    void FxService::Update(const float deltaTime)
    {
        for (const auto& fx : m_effects)
        {
            fx->Update(deltaTime);
        }

        // 완료된 이펙트 제거
        std::erase_if(m_effects, [](const auto& fx) { return fx->IsFinished(); });
    }

    void FxService::Render(IGraphics& graphics) const
    {
        for (auto& fx : m_effects)
        {
            fx->Render(graphics);
        }
    }

    void FxService::Clear()
    {
        m_effects.clear();
    }

    std::pair<int32_t, int32_t> FxService::GetShakeOffset() const noexcept
    {
        for (const auto& fx : m_effects)
        {
            if (const auto* shake = dynamic_cast<ShakeFx*>(fx.get()))
            {
                return {shake->GetOffsetX(), shake->GetOffsetY()};
            }
        }

        return {0, 0};
    }
} // namespace GameLibrary