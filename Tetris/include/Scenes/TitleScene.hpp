//
// Created by Labae on 1/12/26.
//

#ifndef TITLESCENE_HPP
#define TITLESCENE_HPP
#include "Scene/Scene.hpp"

namespace GameLibrary
{
    class IInputProvider;
    class SceneManager;
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
        enum class EMenuItem : int32_t
        {
            SinglePlay,
            BattleMode,
            Settings,
            Exit,
            Count
        };

        void SelectMenuItem() const;
        void MenuUp();
        void MenuDown();

        GameLibrary::IInputProvider* m_input{};
        GameLibrary::SceneManager* m_sceneManager{};
        const GameLibrary::EngineConfig* m_engineConfig{};

        EMenuItem m_selectedMenu{EMenuItem::SinglePlay};

        float m_blinkTimer{};
        bool m_showCursor{true};
    };
} // namespace Tetris

#endif // TITLESCENE_HPP
