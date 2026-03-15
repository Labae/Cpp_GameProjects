//
// Created by Labae on 3/15/26.
//

#ifndef EXPLORATIONSTATE_HPP
#define EXPLORATIONSTATE_HPP

#include "Data/DungeonData.hpp"
#include "States/IState.hpp"
#include "UI/MenuSelector.hpp"
#include "UI/TextBox.hpp"

namespace TextRPG
{
    class DungeonScene;

    class ExplorationState final : public GameLibrary::IState<DungeonScene>
    {
    public:
        void OnEnter(DungeonScene& context) override;
        void Update(DungeonScene& context, float deltaTime) override;
        void Render(DungeonScene& context, GameLibrary::IGraphics& graphics) override;

    private:
        void EnterRoom(const DungeonScene& context);
        void GenerateBranch();
        void ProceedToRoom(const DungeonScene& context);
        static ERoom RollRoomType();

        GameLibrary::TextBox m_textBox{};
        std::unique_ptr<GameLibrary::MenuSelector> m_menu{};

        enum class EPhase : uint8_t
        {
            RoomEntry, // 방 진입
            Branch,    // 갈림길
            Proceeding // 다음 방
        };

        EPhase m_phase{EPhase::RoomEntry};

        ERoom m_leftRoom{};
        ERoom m_rightRoom{};
    };
} // namespace TextRPG

#endif // EXPLORATIONSTATE_HPP
