//
// Created by Labae on 3/15/26.
//

#include "Scenes/States/ExplorationState.hpp"

#include "Scenes/DungeonScene.hpp"
#include "Systems/Logger.hpp"
#include "Utility/Random.hpp"

#include <format>

namespace
{
    constexpr int32_t TEXT_BOX_Y = 80;
    constexpr int32_t TEXT_BOX_WIDTH = 700;
    constexpr int32_t TEXT_BOX_HEIGHT = 200;
    constexpr int32_t MENU_Y = 350;

    [[nodiscard]] std::string GetRoomHint(const TextRPG::ERoom roomType)
    {
        switch (roomType)
        {
        case TextRPG::ERoom::Combat:
            return "You hear growling sounds...";
        case TextRPG::ERoom::Treasure:
            return "Something glitters in the distance...";
        case TextRPG::ERoom::Rest:
            return "A warm breeze flows through...";
        case TextRPG::ERoom::Event:
            return "An strange aura fills the air...";
        case TextRPG::ERoom::Boss:
            return "A powerful presence awaits...";
        default:
            return "The path continues...";
        }
    }

    [[nodiscard]] std::string GetRoomTypeName(const TextRPG::ERoom roomType)
    {
        switch (roomType)
        {
        case TextRPG::ERoom::Combat:
            return "[red]Monster Room[/]";
        case TextRPG::ERoom::Treasure:
            return "[gold]Treasure Room[/]";
        case TextRPG::ERoom::Rest:
            return "[green]Rest Room[/]";
        case TextRPG::ERoom::Event:
            return "[cyan]Mystery Room[/]";
        case TextRPG::ERoom::Boss:
            return "[red]BOSS Room[/]";
        default:
            return "Room";
        }
    }
} // namespace

namespace TextRPG
{
    void ExplorationState::OnEnter(DungeonScene& context)
    {
        const auto& config = context.GetEngineConfig();
        const int32_t textX = (config.screenWidth - TEXT_BOX_WIDTH) / 2;

        m_textBox.SetPosition(textX, TEXT_BOX_Y);
        m_textBox.SetSize(TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT);

        m_menu = std::make_unique<GameLibrary::MenuSelector>(context.GetInput());
        m_menu->SetPosition(config.screenWidth / 2, MENU_Y);
        m_menu->SetStyle(
            {.fontSize = 24, .selectedFontSize = 28, .spacing = 45, .align = GameLibrary::TextAlign::Center});

        EnterRoom(context);
    }

    void ExplorationState::Update(DungeonScene& context, [[maybe_unused]] float deltaTime)
    {
        m_menu->Update();

        switch (m_phase)
        {
        case EPhase::RoomEntry:
        {
            if (m_menu->IsConfirmed())
            {
                ProceedToRoom(context);
            }
            break;
        }
        case EPhase::Branch:
        {
            if (m_menu->IsConfirmed())
            {
                auto& dungeon = context.GetSession().dungeon;
                dungeon.currentRoomType = (m_menu->GetSelectedIndex() == 0) ? m_leftRoom : m_rightRoom;

                ProceedToRoom(context);
            }
            break;
        }
        case EPhase::Proceeding:
            break;
        }
    }
    void ExplorationState::Render([[maybe_unused]] DungeonScene& context, GameLibrary::IGraphics& graphics)
    {
        m_textBox.Render(graphics);
        m_menu->Render(graphics);
    }

    void ExplorationState::EnterRoom(const DungeonScene& context)
    {
        auto& dungeon = context.GetSession().dungeon;

        // 마지막 방 보스
        if (dungeon.currentRoom >= dungeon.roomsPerFloor)
        {
            // 보스 연출
            m_textBox.SetText(std::format("[yellow]Floor {} - Boss Room[/]\n" "\n"
                                          "[red]A powerful presence awaits...[/]\n" "Prepare yourself!",
                                          dungeon.currentFloor));
            m_menu->SetItems({"Enter"});
            return;
        }

        // 짝수 방마다 갈림길
        if (dungeon.currentRoom > 0 and dungeon.currentRoom % 2 == 0)
        {
            // Branch생성
            GenerateBranch();

            m_textBox.SetText(std::format("[yellow]Floor {} - Room {}[/]\n" "\n" "The path splits into two.\n"
                                          " [gray]Left:[/] {}\n" " [gray]Right:[/] {}", dungeon.currentFloor,
                                          dungeon.currentRoom + 1, GetRoomHint(m_leftRoom), GetRoomHint(m_rightRoom)));

            m_menu->SetItems({"Go Left", "Go Right"});
            m_phase = EPhase::Branch;
        }
        else
        {
            dungeon.currentRoomType = RollRoomType();
            m_phase = EPhase::RoomEntry;

            m_textBox.SetText(std::format("[yellow]Floor {} - Room {}[/]\n" "\n" "You enter a {}.\n" "{}",
                                          dungeon.currentFloor, dungeon.currentRoom + 1,
                                          GetRoomTypeName(dungeon.currentRoomType),
                                          GetRoomHint(dungeon.currentRoomType)));

            m_menu->SetItems({"Continue"});
        }
    }

    void ExplorationState::GenerateBranch()
    {
        m_leftRoom = RollRoomType();
        m_rightRoom = RollRoomType();

        // 좌/우가 같으면 다시 굴리기
        while (m_rightRoom == m_leftRoom)
        {
            m_rightRoom = RollRoomType();
        }
    }

    void ExplorationState::ProceedToRoom(const DungeonScene& context)
    {
        auto& dungeon = context.GetSession().dungeon;
        switch (dungeon.currentRoomType)
        {
        case ERoom::Combat:
        case ERoom::Boss:
            // TODO: 배틀 상태로 이동

            // 임시로 다음방 ㄱㄱ
            dungeon.currentRoom++;
            EnterRoom(context);
            break;

        case ERoom::Treasure:
            // TODO: 보물 상태로 이동

            dungeon.currentRoom++;
            EnterRoom(context);
            break;

        case ERoom::Rest:
            // TODO: 휴식 상태로 이동

            dungeon.currentRoom++;
            EnterRoom(context);
            break;

        case ERoom::Event:
            // TODO: 이벤트 상태로 이동

            dungeon.currentRoom++;
            EnterRoom(context);
            break;
        default:
            GameLibrary::Logger::Error("Invalid room type!");
            break;
        }
    }

    ERoom ExplorationState::RollRoomType()
    {
        // Combat: 40, Treasure 20, Rest 20, Event 20
        const auto roll = GameLibrary::Random::Range(1, 100);

        if (roll <= 40)
        {
            return ERoom::Combat;
        }
        if (roll <= 60)
        {
            return ERoom::Treasure;
        }
        if (roll <= 80)
        {
            return ERoom::Rest;
        }
        return ERoom::Event;
    }
} // namespace TextRPG