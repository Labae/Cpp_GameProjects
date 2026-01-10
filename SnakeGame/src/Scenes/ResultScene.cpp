#include "Scenes/ResultScene.hpp"

#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"
#include "Data/GameData.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Scene/SceneManager.hpp"
#include "Services/SaveService.hpp"
#include "Systems/Logger.hpp"

#include <format>

namespace
{
    constexpr auto SAVE_FILE_PATH = "savedata.dat";
    constexpr auto KEY_HIGH_SCORE = "highScore";
} // namespace

ResultScene::ResultScene(const std::string& name, GameLibrary::ServiceContainer& container) : Scene(name, container) {}

void ResultScene::OnEnter()
{
    auto& container = GetContainer();
    m_input = container.Resolve<GameLibrary::IInputProvider>();
    m_sceneManager = container.Resolve<GameLibrary::SceneManager>();
    m_engineConfig = container.Resolve<GameLibrary::EngineConfig>();
    m_gameData = container.Resolve<SnakeGame::GameData>();

    // 최고 점수 갱신 체크
    if (m_gameData->lastScore > m_gameData->highScore)
    {
        m_gameData->highScore = m_gameData->lastScore;
        m_isNewHighScore = true;

        // 즉시 저장
        if (auto* saveSystem = container.Resolve<GameLibrary::SaveService>())
        {
            saveSystem->SetInt(KEY_HIGH_SCORE, m_gameData->highScore);
            if (!saveSystem->SaveToFile(SAVE_FILE_PATH))
            {
                GameLibrary::Logger::Error("Failed to save high score");
            }
        }
    }
    else
    {
        m_isNewHighScore = false;
    }
}

void ResultScene::Update([[maybe_unused]] float deltaTime)
{
    if (m_input->IsKeyPressed(GameLibrary::KeyCode::Space))
    {
        m_sceneManager->LoadScene("Game");
    }
    else if (m_input->IsKeyPressed(GameLibrary::KeyCode::Escape))
    {
        m_sceneManager->LoadScene("Title");
    }
}

void ResultScene::Render(GameLibrary::IGraphics& graphics)
{
    const int32_t centerX = m_engineConfig->screenWidth / 2;

    // 게임 오버 타이틀
    graphics.DrawLabel("GAME OVER", centerX, 200, 64, sf::Color(255, 80, 80, 255), GameLibrary::TextAlign::Center);

    // 최종 점수
    const std::string scoreText = std::format("Score: {}", m_gameData->lastScore);
    graphics.DrawLabel(scoreText, centerX, 320, 36, sf::Color(255, 255, 255, 255), GameLibrary::TextAlign::Center);

    // 최고 점수
    const std::string highScoreText = std::format("High Score: {}", m_gameData->highScore);
    const sf::Color highScoreColor = m_isNewHighScore ? sf::Color(255, 215, 0, 255) : sf::Color(180, 180, 180, 255);
    graphics.DrawLabel(highScoreText, centerX, 370, 28, highScoreColor, GameLibrary::TextAlign::Center);

    // 신기록 표시
    if (m_isNewHighScore)
    {
        graphics.DrawLabel("NEW HIGH SCORE!", centerX, 420, 24, sf::Color(255, 215, 0, 255),
                           GameLibrary::TextAlign::Center);
    }

    // 안내 텍스트
    graphics.DrawLabel("Press SPACE to Play Again", centerX, 520, 24, sf::Color(255, 255, 255, 255),
                       GameLibrary::TextAlign::Center);
    graphics.DrawLabel("Press ESC for Title", centerX, 560, 20, sf::Color(150, 150, 150, 255),
                       GameLibrary::TextAlign::Center);
}