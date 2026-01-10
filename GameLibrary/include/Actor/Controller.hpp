//
// Created by Labae on 1/11/26.
//

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

namespace GameLibrary
{

    class Pawn;

    class Controller
    {
    public:
        Controller() = default;
        virtual ~Controller() = default;

        Controller(const Controller&) = delete;
        Controller(const Controller&&) = delete;
        Controller operator=(const Controller&) = delete;
        Controller operator=(const Controller&&) = delete;

        virtual void Update([[maybe_unused]] float deltaTime) {}

        void Possess(Pawn* pawn);
        void UnPossess();

        [[nodiscard]] Pawn* GetPawn() const noexcept { return m_pawn; }
        [[nodiscard]] bool HasPawn() const noexcept { return m_pawn != nullptr; }

    protected:
        Pawn* m_pawn{};
    };

} // namespace GameLibrary

#endif // CONTROLLER_HPP
