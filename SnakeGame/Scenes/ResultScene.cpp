#include "ResultScene.hpp"
#include "Core/ServiceContainer.hpp"
#include "Core/EngineConfig.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Interfaces/ISceneManager.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Data/GameData.hpp"
#include "Systems/SaveSystem.hpp"
#include "Systems/Logger.hpp"
#include <format>

namespace
{
    constexpr const char* SAVE_FILE_PATH = "savedata.dat";
    constexpr const char* KEY_HIGH_SCORE = "highScore";
}

ResultScene::ResultScene(const std::string& name, GameLibrary::ServiceContainer& container)
    : Scene(name, container)
{
}

void ResultScene::OnEnter()
{
    auto& container = GetContainer();
    m_input = container.Resolve<GameLibrary::IInputProvider>();
    m_sceneManager = container.Resolve<GameLibrary::ISceneManager>();
    m_engineConfig = container.Resolve<GameLibrary::EngineConfig>();
    m_gameData = container.Resolve<SnakeGame::GameData>();

    // 최고 점수 갱신 체크
    if (m_gameData->lastScore > m_gameData->highScore)
    {
        m_gameData->highScore = m_gameData->lastScore;
        m_isNewHighScore = true;

        // 즉시 저장
        if (auto* saveSystem = container.Resolve<GameLibrary::SaveSystem>())
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

void ResultScene::Update(float deltaTime)
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
    int screenWidth = m_engineConfig->screenWidth;
    int centerX = 0;

    // 게임 오버 타이틀
    graphics.DrawLabel(L"GAME OVER", centerX, 200, screenWidth, 64, { 255, 80, 80, 255 }, GameLibrary::TextAlign::Center);

    // 최종 점수
    std::wstring scoreText = std::format(L"Score: {}", m_gameData->lastScore);
    graphics.DrawLabel(scoreText.c_str(), centerX, 320, screenWidth, 36, { 255, 255, 255, 255 }, GameLibrary::TextAlign::Center);

    // 최고 점수
    std::wstring highScoreText = std::format(L"High Score: {}", m_gameData->highScore);
    GameLibrary::Color highScoreColor = m_isNewHighScore ? GameLibrary::Color{ 255, 215, 0, 255 } : GameLibrary::Color{ 180, 180, 180, 255 };
    graphics.DrawLabel(highScoreText.c_str(), centerX, 370, screenWidth, 28, highScoreColor, GameLibrary::TextAlign::Center);

    // 신기록 표시
    if (m_isNewHighScore)
    {
        graphics.DrawLabel(L"NEW HIGH SCORE!", centerX, 420, screenWidth, 24, { 255, 215, 0, 255 }, GameLibrary::TextAlign::Center);
    }

    // 안내 텍스트
    graphics.DrawLabel(L"Press SPACE to Play Again", centerX, 520, screenWidth, 24, { 255, 255, 255, 255 }, GameLibrary::TextAlign::Center);
    graphics.DrawLabel(L"Press ESC for Title", centerX, 560, screenWidth, 20, { 150, 150, 150, 255 }, GameLibrary::TextAlign::Center);
}