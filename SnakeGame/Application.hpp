#pragma once

#include <Windows.h>
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

    [[nodiscard]] int Run(HINSTANCE hInstance);

private:
    void RegisterScenes();
    void LoadSounds();
    void LoadGameData();
    void SaveGameData();

    GameLibrary::Engine m_engine;
};