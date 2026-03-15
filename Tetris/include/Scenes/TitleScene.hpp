//
// Created by Labae on 1/12/26.
//

#ifndef TITLESCENE_HPP
#define TITLESCENE_HPP
#include "Scene/Scene.hpp"
#include "Time/Timer.hpp"
#include "UI/MenuSelector.hpp"

#include <optional>

namespace GameLibrary
{
    class IInputProvider;
    class SceneManager;
    class EventService;
    struct EngineConfig;
} // namespace GameLibrary

namespace Tetris
{
    class TitleScene final : public GameLibrary::Scene
    {
    public:
        TitleScene(const std::string& name, GameLibrary::ServiceContainer& container);

        void OnEnter() override;

        void Update(float deltaTime) override;
        void Render(GameLibrary::IGraphics& graphics) override;

    private:
        void SelectMenuItem() const;

        GameLibrary::IInputProvider* m_input{};
        GameLibrary::SceneManager* m_sceneManager{};
        GameLibrary::EventService* m_eventService{};
        const GameLibrary::EngineConfig* m_engineConfig{};

        std::optional<GameLibrary::MenuSelector> m_menu{};

        GameLibrary::Timer m_blinkTimer;
        bool m_showCursor{true};
    };
} // namespace Tetris

#endif // TITLESCENE_HPP
