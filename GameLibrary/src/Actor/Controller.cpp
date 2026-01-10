//
// Created by Labae on 1/11/26.
//

#include "Actor/Controller.hpp"

#include "Actor/Pawn.hpp"

namespace GameLibrary
{
    void Controller::Possess(Pawn* pawn)
    {
        if (m_pawn == pawn)
        {
            return;
        }

        UnPossess();

        m_pawn = pawn;
        if (m_pawn)
        {
            m_pawn->SetController(this);
        }
    }

    void Controller::UnPossess()
    {
        if (m_pawn)
        {
            m_pawn->SetController(nullptr);
            m_pawn = nullptr;
        }
    }
} // namespace GameLibrary