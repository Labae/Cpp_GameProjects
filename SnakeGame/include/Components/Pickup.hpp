#pragma once

#include "../../../GameLibrary/include/Components/Component.hpp"
#include "Services/EventService.hpp"

namespace GameLibrary
{
    class IGraphics;
    class ICollidable;
    class EventService;
    class Actor;
} // namespace GameLibrary

class Pickup final : public GameLibrary::Component
{
public:
    Pickup(GameLibrary::Actor* owner, GameLibrary::EventService& eventSystem, int32_t gridSize);

    void Render(GameLibrary::IGraphics& graphics) override;

    void OnCollision(const GameLibrary::Actor* other) const;

private:
    GameLibrary::EventService& m_eventSystem;
    int32_t m_gridSize{};
};