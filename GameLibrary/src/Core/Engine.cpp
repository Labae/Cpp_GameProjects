#include "Core/Engine.hpp"

#include "../../include/Services/AudioService.hpp"
#include "../../include/Services/ConfigService.hpp"
#include "../../include/Services/EventService.hpp"
#include "../../include/Services/FxService.hpp"
#include "../../include/Services/GraphicsService.hpp"
#include "../../include/Services/InputService.hpp"
#include "../../include/Services/PhysicsService.hpp"
#include "../../include/Services/SaveService.hpp"
#include "../../include/Services/TimeService.hpp"
#include "../../include/Systems/Logger.hpp"
#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Platform/Window.hpp"
#include "Scene/Scene.hpp"
#include "Scene/SceneManager.hpp"
#include "Services/ResourceService.hpp"

namespace GameLibrary
{
    bool Engine::Init(const EngineConfig& config)
    {
        m_config = config;

        RegisterServices();
        InitializeServices();

        m_isRunning = true;
        Logger::Info("Engine initialized");
        return true;
    }

    void Engine::Run()
    {
        while (m_isRunning)
        {
            if (!m_window->ProcessMessages())
            {
                m_isRunning = false;
                break;
            }

            Update();
            Render();
        }
    }

    void Engine::Shutdown() noexcept
    {
        m_isRunning = false;

        if (m_resourceService)
        {
            m_resourceService->UnloadAll();
        }

        if (m_soundService)
        {
            m_soundService->StopAll();
            m_soundService->StopBGM();
        }

        if (m_sceneManager)
        {
            if (auto* scene = m_sceneManager->GetCurrentScene())
            {
                scene->OnExit();
                scene->Clear();
            }
        }

        if (m_eventService)
        {
            m_eventService->Clear();
        }

        if (m_window)
        {
            m_window->Destroy();
        }
    }

    void Engine::ReloadConfig() const
    {
        if (!m_configService)
        {
            return;
        }

        m_configService->Clear();

        if (!m_config.configPath.empty())
        {
            if (!m_configService->LoadFromFile(m_config.configPath))
            {
                Logger::Warning("Failed to load config: " + m_config.configPath);
            }
        }

        // 참고: screenWidth/screenHeight는 Window 재생성이 필요해서 런타임에 적용 안됨
    }

    void Engine::RegisterServices()
    {
        m_container.RegisterInstance<EngineConfig>(std::make_shared<EngineConfig>(m_config));
        m_container.Register<Window, Window>();
        m_container.Register<TimeService, TimeService>();
        m_container.Register<IInputProvider, InputService>();
        m_container.Register<IGraphics, GraphicsService>();
        m_container.Register<PhysicsService, PhysicsService>();
        m_container.Register<SceneManager, SceneManager>();
        m_container.Register<SaveService, SaveService>();
        m_container.Register<ConfigService, ConfigService>();
        m_container.Register<EventService, EventService>();
        m_container.Register<AudioService, AudioService>();
        m_container.Register<FxService, FxService>();
        m_container.Register<ResourceService, ResourceService>();
    }

    void Engine::InitializeServices()
    {
        m_configService = TryInitialize<ConfigService>();
        if (!m_config.configPath.empty())
        {
            if (!m_configService->LoadFromFile(m_config.configPath))
            {
                Logger::Warning("Failed to load config: " + m_config.configPath);
            }
        }

        m_config.screenWidth = m_configService->GetInt("screenWidth", m_config.screenWidth);
        m_config.screenHeight = m_configService->GetInt("screenHeight", m_config.screenHeight);

        m_window = TryInitialize<Window>();
        m_inputProvider = TryInitialize<IInputProvider>();
        m_graphics = TryInitialize<IGraphics>();
        m_sceneManager = TryInitialize<SceneManager>();
        m_timeService = TryInitialize<TimeService>();
        m_physicsService = TryInitialize<PhysicsService>();
        m_eventService = TryInitialize<EventService>();
        m_saveService = TryInitialize<SaveService>();
        m_soundService = TryInitialize<AudioService>();
        m_fxService = TryInitialize<FxService>();
        m_resourceService = TryInitialize<ResourceService>();
    }

    void Engine::Update() const
    {
        m_timeService->Update();
        m_inputProvider->Update();

        ProcessGlobalInput();

        const float deltaTime = m_timeService->GetDeltaTime();
        m_sceneManager->Update(deltaTime);

        if (m_physicsService)
        {
            m_physicsService->CheckCollisions();
        }
    }

    void Engine::Render() const
    {
        m_graphics->Clear({0, 0, 0, 255});
        m_sceneManager->Render(*m_graphics);
        m_graphics->Present();
    }

    void Engine::ProcessGlobalInput() const
    {
        // F5: Config 리로드 후 현재 씬 재시작
        if (m_inputProvider->IsKeyPressed(KeyCode::F5))
        {
            ReloadConfig();

            if (const auto* currentScene = m_sceneManager->GetCurrentScene())
            {
                m_sceneManager->LoadScene(currentScene->GetName());
            }
        }
    }

    template <typename T> T* Engine::TryInitialize()
    {
        auto* service = m_container.Resolve<T>();
        if (!service)
        {
            Logger::Error("Failed to resolve service");
            return nullptr;
        }

        m_container.TryInit(std::type_index(typeid(T)), service);

        return service;
    }
} // namespace GameLibrary