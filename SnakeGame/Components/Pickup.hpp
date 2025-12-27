#pragma once

#include "GameObject/Component.hpp"
#include "GameObject/Transform.hpp"
#include "Systems/EventSystem.hpp"

namespace GameLibrary
{
    class IGraphics;
}

class Pickup : public GameLibrary::Component
{
public:
    Pickup(GameLibrary::Transform& transform, GameLibrary::EventSystem& eventSystem, int gridSize);

    void Init() override;
    void Render(GameLibrary::IGraphics& graphics) override;

    [[nodiscard]] GameLibrary::Transform& GetTransform() const noexcept { return m_transform; }

private:
    void OnSnakeMoved(const struct SnakeMovedEvent& event);

    GameLibrary::Transform& m_transform;
    GameLibrary::EventSystem& m_eventSystem;
    int m_gridSize;

    GameLibrary::SubscriptionToken m_snakeMovedToken;
};