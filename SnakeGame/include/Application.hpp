#pragma once

#include "Core/Engine.hpp"

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
    void LoadSounds();
    void LoadGameData();
    void SaveGameData();

    GameLibrary::Engine m_engine{};
};