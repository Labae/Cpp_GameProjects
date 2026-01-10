//
// Created by Labae on 1/11/26.
//

#include "Controllers/SnakePlayerController.hpp"

#include "Actor/GridMovementComponent.hpp"
#include "Actor/Pawn.hpp"
#include <SFML/System/Vector2.hpp>

namespace SnakeGame
{
     SnakePlayerController::SnakePlayerController(GameLibrary::IInputProvider& input)
         : m_input(input)
    {

    }

    void SnakePlayerController::Update([[maybe_unused]] const float deltaTime)
     {
         if (not HasPawn())
         {
             return;
         }

         // 반대 방향 입력 차단
         sf::Vector2f currentDirection {1.0f, 0.0f};
         if (const auto* movement = m_pawn->GetComponent<GameLibrary::GridMovementComponent>())
         {
             currentDirection = movement->GetDirection();
         }

         // 입력 처리
         if (m_input.IsKeyPressed(GameLibrary::KeyCode::Up) && currentDirection.y == 0)
         {
             m_pawn->AddMovementInput({0.0f, -1.0f});
         }
         if (m_input.IsKeyPressed(GameLibrary::KeyCode::Down) && currentDirection.y == 0)
         {
             m_pawn->AddMovementInput({0.0f, 1.0f});
         }
         if (m_input.IsKeyPressed(GameLibrary::KeyCode::Right) && currentDirection.x == 0)
         {
             m_pawn->AddMovementInput({1.0f, 0.0f});
         }
         if (m_input.IsKeyPressed(GameLibrary::KeyCode::Left) && currentDirection.x == 0)
         {
             m_pawn->AddMovementInput({-1.0f, 0.0f});
         }
     }


}