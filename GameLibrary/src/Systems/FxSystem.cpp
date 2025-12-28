#include "Systems/FxSystem.hpp"

#include "Fx/Fx.hpp"
#include "Fx/ShakeFx.hpp"

#include <algorithm>

namespace GameLibrary
{
    void FxSystem::Update(float deltaTime)
    {
        for (auto& fx : m_effects)
        {
            fx->Update(deltaTime);
        }

        // 완료된 이펙트 제거
        m_effects.erase(
            std::remove_if(m_effects.begin(), m_effects.end(), [](const auto& fx) { return fx->IsFinished(); }),
            m_effects.end());
    }

    void FxSystem::Render(IGraphics& graphics)
    {
        for (auto& fx : m_effects)
        {
            fx->Render(graphics);
        }
    }

    void FxSystem::Clear()
    {
        m_effects.clear();
    }

    std::pair<int32_t, int32_t> FxSystem::GetShakeOffset() const noexcept
    {
        for (const auto& fx : m_effects)
        {
            if (auto* shake = dynamic_cast<ShakeFx*>(fx.get()))
            {
                return {shake->GetOffsetX(), shake->GetOffsetY()};
            }
        }

        return {0, 0};
    }
} // namespace GameLibrary