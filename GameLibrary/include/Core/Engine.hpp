#pragma once

#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"

#include <memory>
#include <string>

namespace GameLibrary
{
    class Window;
    class EventSystem;
    class SaveSystem;
    class ConfigSystem;
    class SoundSystem;
    class FxSystem;

    class IGraphics;
    class IInputProvider;
    class ITimeProvider;
    class IPhysicsSystem;
    class ISceneManager;

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
        [[nodiscard]] ServiceContainer& GetContainer() noexcept { return m_container; }

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

        IGraphics* m_graphics{};
        IInputProvider* m_inputProvider{};
        ITimeProvider* m_timeProvider{};
        IPhysicsSystem* m_physicsSystem{};
        ISceneManager* m_sceneManager{};

        EventSystem* m_eventSystem{};
        SaveSystem* m_saveSystem{};
        SoundSystem* m_soundSystem{};
        FxSystem* m_fxSystem{};
    };
} // namespace GameLibrary