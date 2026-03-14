#pragma once

#include "Core/Engine.hpp"
#include "Services/EventService.hpp"

#include <cstdint>

namespace Tetris
{
    class Application
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
        void LoadConfig();

        GameLibrary::Engine m_engine{};
        GameLibrary::SubscriptionToken m_quitToken{};
    };
} // namespace Tetris