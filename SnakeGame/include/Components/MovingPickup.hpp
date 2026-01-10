//
// Created by Labae on 1/11/26.
//

#ifndef MOVINGPICKUP_HPP
#define MOVINGPICKUP_HPP
#include "Actor/Component.hpp"

#include <cstdint>

namespace GameLibrary
{
    class EventService;
    struct EngineConfig;
    class AutoGridMovementComponent;
} // namespace GameLibrary

class MovingPickup final : public GameLibrary::Component
{
public:
    MovingPickup(GameLibrary::Actor* owner, GameLibrary::EventService& eventService,
                 const GameLibrary::EngineConfig& engineConfig, const int32_t gridSize, int32_t score = 3);

    void Init() override;
    void Update(float deltaTime) override;
    void Render(GameLibrary::IGraphics& graphics) override;

    void OnCollision(const GameLibrary::Actor* other);

    [[nodiscard]] bool IsEaten() const noexcept { return m_isEaten; }

private:
    void BounceOffWalls() const;

    GameLibrary::EventService& m_eventService;
    const GameLibrary::EngineConfig& m_engineConfig;
    GameLibrary::AutoGridMovementComponent* m_movement{};

    int32_t m_gridSize{};
    int32_t m_score{};
    bool m_isEaten{};
};

#endif // MOVINGPICKUP_HPP
