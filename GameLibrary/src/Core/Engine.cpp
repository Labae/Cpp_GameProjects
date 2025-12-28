#include "Core/Engine.hpp"

#include "Core/EngineConfig.hpp"
#include "Core/ServiceContainer.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Interfaces/IPhysicsSystem.hpp"
#include "Interfaces/ISceneManager.hpp"
#include "Interfaces/ITimeProvider.hpp"
#include "Platform/Window.hpp"
#include "Scene/Scene.hpp"
#include "Scene/SceneManager.hpp"
#include "Systems/ConfigSystem.hpp"
#include "Systems/EventSystem.hpp"
#include "Systems/FxSystem.hpp"
#include "Systems/GraphicsSystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Systems/Logger.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/SaveSystem.hpp"
#include "Systems/SoundSystem.hpp"
#include "Systems/TimeSystem.hpp"

#include <concepts>

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

        if (m_soundSystem)
        {
            m_soundSystem->StopAll();
            m_soundSystem->StopBGM();
        }

        if (m_sceneManager)
        {
            if (auto* scene = m_sceneManager->GetCurrentScene())
            {
                scene->OnExit();
                scene->Clear();
            }
        }

        if (m_eventSystem)
        {
            m_eventSystem->Clear();
        }

        if (m_window)
        {
            m_window->Destroy();
        }
    }

    void Engine::ReloadConfig()
    {
        if (!m_configSystem)
        {
            return;
        }

        m_configSystem->Clear();

        if (!m_config.configPath.empty())
        {
            if (!m_configSystem->LoadFromFile(m_config.configPath))
            {
                Logger::Warning("Failed to load config: " + m_config.configPath);
            }
        }

        // 참고: screenWidth/screenHeight는 Window 재생성이 필요해서 런타임에 적용 안됨
    }

    void Engine::RegisterServices()
    {
        m_container.Register<ITimeProvider, TimeSystem>();
        m_container.Register<IInputProvider, InputSystem>();
        m_container.Register<IGraphics, GraphicsSystem>();
        m_container.Register<IPhysicsSystem, PhysicsSystem>();
        m_container.Register<ISceneManager, SceneManager>();
        m_container.Register<SaveSystem, SaveSystem>();
        m_container.Register<ConfigSystem, ConfigSystem>();
        m_container.Register<EventSystem, EventSystem>();
        m_container.Register<SoundSystem, SoundSystem>();
        m_container.Register<FxSystem, FxSystem>();
    }

    void Engine::InitializeServices()
    {
        m_configSystem = TryInitialize<ConfigSystem>();
        if (!m_config.configPath.empty())
        {
            if (!m_configSystem->LoadFromFile(m_config.configPath))
            {
                Logger::Warning("Failed to load config: " + m_config.configPath);
            }
        }

        m_config.screenWidth = m_configSystem->GetInt("screenWidth", m_config.screenWidth);
        m_config.screenHeight = m_configSystem->GetInt("screenHeight", m_config.screenHeight);

        m_window = TryInitialize<Window>();
        m_inputProvider = TryInitialize<InputSystem>();
        m_graphics = TryInitialize<GraphicsSystem>();
        m_sceneManager = TryInitialize<SceneManager>();
        m_timeProvider = TryInitialize<TimeSystem>();
        m_physicsSystem = TryInitialize<PhysicsSystem>();
        m_eventSystem = TryInitialize<EventSystem>();
        m_saveSystem = TryInitialize<SaveSystem>();
        m_soundSystem = TryInitialize<SoundSystem>();
        m_fxSystem = TryInitialize<FxSystem>();
    }

    void Engine::Update()
    {
        m_timeProvider->Update();
        m_inputProvider->Update();

        ProcessGlobalInput();

        float deltaTime = m_timeProvider->GetDeltaTime();
        m_sceneManager->Update(deltaTime);

        if (m_physicsSystem)
        {
            m_physicsSystem->CheckCollisions();
        }
    }

    void Engine::Render()
    {
        m_graphics->Clear({0, 0, 0, 255});
        m_sceneManager->Render(*m_graphics);
        m_graphics->Present();
    }

    void Engine::ProcessGlobalInput()
    {
        // F5: Config 리로드 후 현재 씬 재시작
        if (m_inputProvider->IsKeyPressed(KeyCode::F5))
        {
            ReloadConfig();

            if (auto* currentScene = m_sceneManager->GetCurrentScene())
            {
                m_sceneManager->LoadScene(currentScene->GetName());
            }
        }
    }

    template <typename T> T* Engine::TryInitialize()
    {
        auto* service = m_container.Resolve<T>();
        if constexpr (std::derived_from<T, IInitializable>)
        {
            service->Init(m_container);
        }

        return service;
    }
} // namespace GameLibrary