//
// Created by Labae on 3/15/26.
//

#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include "Components/LinearMovementComponent.hpp"
#include "Core/Engine.hpp"
#include "Services/EventService.hpp"

#include <cstdint>

namespace TextRPG
{
    class Application final
    {
    public:
        Application() = default;
        ~Application() = default;

        Application(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;

        [[nodiscard]] int32_t Run();

    private:
        void RegisterScenes();
        void SubscribeEvents();
        void LoadResources();

        GameLibrary::Engine m_engine{};
        GameLibrary::SubscriptionToken m_quitToken{};
    };
} // namespace TextRPG

#endif // APPLICATION_HPP
