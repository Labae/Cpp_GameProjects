//
// Created by Labae on 3/15/26.
//

#ifndef TITLESCENE_HPP
#define TITLESCENE_HPP
#include "Core/EngineConfig.hpp"
#include "Scene/Scene.hpp"
#include "Scene/SceneManager.hpp"
#include "Services/EventService.hpp"
#include "UI/MenuSelector.hpp"

namespace TextRPG
{
    class TitleScene final : public GameLibrary::Scene
    {
    public:
        TitleScene(const std::string& name, GameLibrary::ServiceContainer& container);

        void OnEnter() override;
        void Update(float deltaTime) override;
        void Render(GameLibrary::IGraphics& graphics) override;

    private:
        GameLibrary::IInputProvider* m_input{};
        GameLibrary::SceneManager* m_sceneManager{};
        GameLibrary::EventService* m_eventService{};
        GameLibrary::EngineConfig* m_engineConfig{};

        std::unique_ptr<GameLibrary::MenuSelector> m_menu{};
    };
} // namespace TextRPG

#endif //TITLESCENE_HPP
