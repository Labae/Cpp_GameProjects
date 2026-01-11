//
// Created by Labae on 1/12/26.
//

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Core/Engine.hpp"

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
        void LoadResources();

        GameLibrary::Engine m_engine{};
    };
} // namespace Tetris

#endif // APPLICATION_HPP
