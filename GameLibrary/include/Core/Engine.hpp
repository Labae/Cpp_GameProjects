#pragma once

#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"

namespace GameLibrary
{
    class Window;
    class EventService;
    class SaveService;
    class ConfigService;
    class AudioService;
    class FxService;
    class ResourceService;
    class PhysicsService;
    class TimeService;

    class IGraphics;
    class IInputProvider;
    class SceneManager;

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

        void ReloadConfig() const;

        [[nodiscard]] bool IsRunning() const noexcept { return m_isRunning; }
        [[nodiscard]] ServiceContainer& GetContainer() noexcept { return m_container; }

    private:
        void RegisterServices();
        void InitializeServices();

        template <typename T> [[nodiscard]] T* TryInitialize();

        void Update() const;
        void Render() const;
        void ProcessGlobalInput() const;

        bool m_isRunning = false;
        EngineConfig m_config{};
        ServiceContainer m_container{};

        Window* m_window{};

        IGraphics* m_graphics{};
        IInputProvider* m_inputProvider{};

        ConfigService* m_configService{};
        TimeService* m_timeService{};
        PhysicsService* m_physicsService{};
        SceneManager* m_sceneManager{};
        EventService* m_eventService{};
        SaveService* m_saveService{};
        AudioService* m_soundService{};
        FxService* m_fxService{};
        ResourceService* m_resourceService{};
    };
} // namespace GameLibrary