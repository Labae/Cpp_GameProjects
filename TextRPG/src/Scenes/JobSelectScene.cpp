//
// Created by Labae on 3/15/26.
//

#include "Scenes/JobSelectScene.hpp"

#include "Core/ServiceContainer.hpp"
#include "Data/GameDataLoader.hpp"
#include "Data/GameSession.hpp"

#include <format>

namespace
{
    constexpr int32_t TITLE_Y = 80;
    constexpr int32_t TITLE_FONT_SIZE = 40;
    constexpr int32_t MENU_Y = 200;
    constexpr int32_t DESC_BOX_Y = 380;
    constexpr int32_t DESC_BOX_WIDTH = 500;
    constexpr int32_t DESC_BOX_HEIGHT = 160;
    constexpr int32_t HINT_Y = 560;

    constexpr sf::Color COLOR_TITLE{255, 255, 255, 255};
    constexpr sf::Color COLOR_HINT{100, 100, 100, 255};
} // namespace

namespace TextRPG
{
    JobSelectScene::JobSelectScene(const std::string& name, GameLibrary::ServiceContainer& container)
        : Scene(name, container)
    {
    }

    void JobSelectScene::OnEnter()
    {
        auto& container = GetContainer();
        m_input = container.Resolve<GameLibrary::IInputProvider>();
        m_sceneManager = container.Resolve<GameLibrary::SceneManager>();
        m_engineConfig = container.Resolve<GameLibrary::EngineConfig>();

        m_jobs = GameDataLoader::LoadJobs("data/jobs.yaml");

        std::vector<std::string> jobNames;
        jobNames.reserve(m_jobs.size());
        for (const auto& job : m_jobs)
        {
            jobNames.emplace_back(job.name);
        }

        m_menu = std::make_unique<GameLibrary::MenuSelector>(*m_input);
        const int32_t centerX = m_engineConfig->screenWidth / 2;
        m_menu->SetItems(jobNames);
        m_menu->SetPosition(centerX, MENU_Y);
        m_menu->SetStyle(
            {.fontSize = 28, .selectedFontSize = 32, .spacing = 50, .align = GameLibrary::TextAlign::Center});

        const int32_t descX = (m_engineConfig->screenWidth - DESC_BOX_WIDTH) / 2;
        m_descBox.SetPosition(descX, DESC_BOX_Y);
        m_descBox.SetSize(DESC_BOX_WIDTH, DESC_BOX_HEIGHT);

        m_lastIndex = -1;
        UpdateDescription();
    }

    void JobSelectScene::Update([[maybe_unused]] float deltaTime)
    {
        m_menu->Update();

        // 커서가 움직이면 설명 갱신
        if (m_menu->GetSelectedIndex() != m_lastIndex)
        {
            UpdateDescription();
        }

        if (m_menu->IsConfirmed())
        {
            SelectJob();
        }

        if (m_menu->IsCancelled())
        {
            m_sceneManager->LoadScene("Title");
        }
    }

    void JobSelectScene::Render(GameLibrary::IGraphics& graphics)
    {
        const int32_t centerX = m_engineConfig->screenWidth / 2;

        // 타이틀
        graphics.DrawLabel("Choose Your Class", centerX, TITLE_Y, TITLE_FONT_SIZE, COLOR_TITLE,
                           GameLibrary::TextAlign::Center);

        // 직업 메뉴
        m_menu->Render(graphics);

        // 설명 박스
        m_descBox.Render(graphics);

        // 힌트
        graphics.DrawLabel("Enter to Select, ESC to Back", centerX, HINT_Y, 16, COLOR_HINT,
                           GameLibrary::TextAlign::Center);
    }

    void JobSelectScene::UpdateDescription()
    {
        m_lastIndex = m_menu->GetSelectedIndex();
        const auto& job = m_jobs[m_lastIndex];

        std::string desc = std::format("[red]HP:[/] {}  [blue]MP:[/] {}  [orange]ATK:[/] {}  [cyan]DEF:[/] {}\n"
                                       "\n"
                                       "[gold]Skills:[/]",
                                       job.baseHp, job.baseMp, job.baseAttack, job.baseDefense);

        for (const auto& skill : job.skills)
        {
            desc += std::format("\n  [gray]{} - {} (MP: {})[/]", skill.name, skill.description, skill.mpCost);
        }

        m_descBox.SetText(desc);
    }

    void JobSelectScene::SelectJob() const
    {
        const auto& job = m_jobs[m_menu->GetSelectedIndex()];

        // PlayerData 생성
        const auto sessionData = std::make_shared<GameSessionData>();
        auto& player = sessionData->player;
        player.jobName = job.name;
        player.hp = job.baseHp;
        player.maxHp = job.baseHp;
        player.mp = job.baseMp;
        player.maxMp = job.baseMp;
        player.attack = job.baseAttack;
        player.defense = job.baseDefense;
        player.skills = job.skills;

        GetContainer().RegisterInstance<GameSessionData>(sessionData);

        m_sceneManager->LoadScene("Dungeon");
    }

} // namespace TextRPG
