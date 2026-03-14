//
// Created by Labae on 3/14/26.
//

#ifndef ISTATE_HPP
#define ISTATE_HPP

namespace GameLibrary
{
    template <typename TContext>
    class IState
    {
    public:
        virtual ~IState() = default;

        virtual void OnEnter([[maybe_unused]] TContext& context) {}
        virtual void OnExit([[maybe_unused]] TContext& context) {}
        virtual void Update([[maybe_unused]] TContext& context, [[maybe_unused]] float deltaTime) {}
    };
} // namespace GameLibrary


#endif //ISTATE_HPP
