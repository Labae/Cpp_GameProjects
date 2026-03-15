//
// Created by Labae on 3/15/26.
//

#ifndef JOBSELECTSCENE_HPP
#define JOBSELECTSCENE_HPP
#include "Core/EngineConfig.hpp"
#include "Data/JobConfig.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Scene/Scene.hpp"
#include "Scene/SceneManager.hpp"
#include "UI/MenuSelector.hpp"
#include "UI/TextBox.hpp"

namespace TextRPG
{
    class JobSelectScene final : public GameLibrary::Scene
    {
    public:
        JobSelectScene(const std::string& name, GameLibrary::ServiceContainer& container);

        void OnEnter() override;
        void Update(float deltaTime) override;
        void Render(GameLibrary::IGraphics& graphics) override;

    private:
        void UpdateDescription();
        void SelectJob() const;

        GameLibrary::IInputProvider* m_input{};
        GameLibrary::SceneManager* m_sceneManager{};
        GameLibrary::EngineConfig* m_engineConfig{};

        std::vector<JobConfig> m_jobs{};
        int32_t m_lastIndex = -1;

        std::unique_ptr<GameLibrary::MenuSelector> m_menu{};
        GameLibrary::TextBox m_descBox{};
    };
} // namespace TextRPG

#endif // JOBSELECTSCENE_HPP
