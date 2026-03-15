//
// Created by Labae on 3/15/26.
//

#ifndef DUNGEONSCENE_HPP
#define DUNGEONSCENE_HPP
#include "Core/EngineConfig.hpp"
#include "Data/GameSession.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Scene/Scene.hpp"
#include "Scene/SceneManager.hpp"
#include "States/StateMachine.hpp"

namespace TextRPG
{
    class DungeonScene final : public GameLibrary::Scene
    {
    public:
        DungeonScene(const std::string& name, GameLibrary::ServiceContainer& container);

        void OnEnter() override;
        void OnExit() override;
        void Update(float deltaTime) override;
        void Render(GameLibrary::IGraphics& graphics) override;

        [[nodiscard]] GameLibrary::EngineConfig& GetEngineConfig() const noexcept { return *m_engineConfig; }
        [[nodiscard]] GameLibrary::IInputProvider& GetInput() const noexcept { return *m_input; }
        [[nodiscard]] GameSessionData& GetSession() const noexcept { return *m_gameSession; }

    private:
        GameLibrary::StateMachine<DungeonScene> m_stateMachine{*this};

        GameLibrary::IInputProvider* m_input{};
        GameLibrary::SceneManager* m_sceneManager{};
        GameLibrary::EngineConfig* m_engineConfig{};
        GameSessionData* m_gameSession{};
    };
} // namespace TextRPG

#endif // DUNGEONSCENE_HPP
