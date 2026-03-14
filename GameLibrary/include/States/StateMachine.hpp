//
// Created by Labae on 3/14/26.
//

#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP
#pragma once

#include "IState.hpp"

#include <cassert>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>

namespace GameLibrary
{
    template <typename TContext> class StateMachine
    {
    public:
        explicit StateMachine(TContext& context) noexcept : m_context(context) {}
        ~StateMachine() = default;

        StateMachine(const StateMachine&) = delete;
        StateMachine(StateMachine&&) = delete;
        StateMachine& operator=(const StateMachine&) = delete;
        StateMachine& operator=(StateMachine&&) = delete;

        template <typename TState, typename... TArgs> TState& AddState(TArgs&&... args)
        {
            static_assert(std::is_base_of_v<IState<TContext>, TState>, "TState must derive from IState<TContext>");

            const auto key = std::type_index(typeid(TState));
            auto state = std::make_unique<TState>(std::forward<TArgs>(args)...);
            auto* ptr = state.get();
            m_states[key] = std::move(state);

            return *ptr;
        }

        template <typename TState> void ChangeState()
        {
            const auto key = std::type_index(typeid(TState));
            const auto it = m_states.find(key);
            assert(it != m_states.end() && "State not registered");

            if (m_currentState)
            {
                m_currentState->OnExit(m_context);
            }

            m_currentState = it->second.get();
            m_currentState->OnEnter(m_context);
        }

        void Update(const float deltaTime)
        {
            if (m_currentState)
            {
                m_currentState->Update(m_context, deltaTime);
            }
        }

        template <typename TState> [[nodiscard]] bool IsInState() const noexcept
        {
            if (not m_currentState)
            {
                return false;
            }

            const auto key = std::type_index(typeid(TState));
            const auto it = m_states.find(key);
            return it != m_states.end() && it->second.get() == m_currentState;
        }

        template <typename TState> [[nodiscard]] TState* GetState() const noexcept
        {
            const auto key = std::type_index(typeid(TState));
            const auto it = m_states.find(key);
            if (it == m_states.end())
            {
                return nullptr;
            }
            return static_cast<TState*>(it->second.get());
        }

        [[nodiscard]] bool HasState() const noexcept { return m_currentState != nullptr; }

    private:
        TContext& m_context;
        std::unordered_map<std::type_index, std::unique_ptr<IState<TContext>>> m_states{};
        IState<TContext>* m_currentState{};
    };
} // namespace GameLibrary
#endif // STATEMACHINE_HPP
