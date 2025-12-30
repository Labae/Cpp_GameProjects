#pragma once

#include "GameObject/Component.hpp"
#include "GameObject/Transform.hpp"
#include "Systems/EventSystem.hpp"

namespace GameLibrary
{
    class IGraphics;
    class ICollidable;
    class EventSystem;
} // namespace GameLibrary

class Pickup : public GameLibrary::Component
{
public:
    Pickup(GameLibrary::Transform& transform, GameLibrary::EventSystem& eventSystem, int32_t gridSize);

    void Render(GameLibrary::IGraphics& graphics) override;

    void OnCollision(GameLibrary::ICollidable* other);

    [[nodiscard]] GameLibrary::Transform& GetTransform() const noexcept { return m_transform; }

private:
    GameLibrary::Transform& m_transform;
    GameLibrary::EventSystem& m_eventSystem;
    int32_t m_gridSize{};
};