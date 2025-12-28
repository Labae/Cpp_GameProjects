#pragma once

#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"

#include <memory>
#include <string>

namespace GameLibrary
{
    class Window;
    class TimeSystem;
    class InputSystem;
    class GraphicsSystem;
    class SceneManager;
    class PhysicsSystem;
    class EventSystem;
    class SaveSystem;
    class ConfigSystem;
    class SoundSystem;
    class FxSystem;

    class Engine
    {
    public:
        Engine() = default;
        ~Engine() = default;

        Engine(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine& operator=(Engine&&) = delete;

        [[nodiscard]] bool Init(const EngineConfig& config);
        void Run();
        void Shutdown() noexcept;
        void Quit() noexcept { m_isRunning = false; }

        void ReloadConfig();

        [[nodiscard]] bool IsRunning() const noexcept { return m_isRunning; }

    private:
        void RegisterServices();
        void InitializeServices();

        template <typename T> [[nodiscard]] T* TryInitialize();

        void Update();
        void Render();
        void ProcessGlobalInput();

        bool m_isRunning = false;
        EngineConfig m_config{};
        ServiceContainer m_container{};

        ConfigSystem* m_configSystem{};
        Window* m_window{};
        GraphicsSystem* m_graphics{};
        InputSystem* m_inputProvider{};
        TimeSystem* m_timeProvider{};
        PhysicsSystem* m_physicsSystem{};
        SceneManager* m_sceneManager{};

        EventSystem* m_eventSystem{};
        SaveSystem* m_saveSystem{};
        SoundSystem* m_soundSystem{};
        FxSystem* m_fxSystem{};
    };
} // namespace GameLibrary