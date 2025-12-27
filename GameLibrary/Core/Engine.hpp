#pragma once

#include <Windows.h>
#include <string>
#include "ServiceContainer.hpp"

namespace GameLibrary
{
    class Window;
    class TimeSystem;
    class InputSystem;
    class GraphicsSystem;
    class SceneManager;
    class IPhysicsSystem;
    class EventSystem;
    class SaveSystem;
    class ConfigSystem;
    class SoundSystem;

    class Engine
    {
    public:
        Engine() = default;
        ~Engine() = default;

        Engine(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine& operator=(Engine&&) = delete;

        [[nodiscard]] bool Init(HINSTANCE hInstance, const wchar_t* title, const char* configPath = nullptr);
        void Run();
        void Shutdown() noexcept;
        void Quit() noexcept { m_isRunning = false; }

        void ReloadConfig();

        [[nodiscard]] bool IsRunning() const noexcept { return m_isRunning; }
        [[nodiscard]] ServiceContainer& GetContainer() noexcept { return m_container; }
        [[nodiscard]] SceneManager* GetSceneManager() const noexcept { return m_sceneManager; }
        [[nodiscard]] SaveSystem* GetSaveSystem() const noexcept { return m_saveSystem; }
        [[nodiscard]] ConfigSystem* GetConfigSystem() const noexcept { return m_configSystem; }
        [[nodiscard]] SoundSystem* GetSoundSystem() const noexcept { return m_soundSystem; }

    private:
        void Update();
        void Render();
        void ProcessGlobalInput();

        bool m_isRunning = false;

        std::string m_configPath;

        ServiceContainer m_container;
        Window* m_window = nullptr;
        TimeSystem* m_timeSystem = nullptr;
        InputSystem* m_inputSystem = nullptr;
        GraphicsSystem* m_graphicsSystem = nullptr;
        SceneManager* m_sceneManager = nullptr;
        IPhysicsSystem* m_physicsSystem = nullptr;
        EventSystem* m_eventSystem = nullptr;
        SaveSystem* m_saveSystem = nullptr;
        ConfigSystem* m_configSystem = nullptr;
        SoundSystem* m_soundSystem = nullptr;
    };
}