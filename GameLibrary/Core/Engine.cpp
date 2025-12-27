#include "Engine.hpp"
#include "Platform/Windows/Window.hpp"
#include "Systems/TimeSystem.hpp"
#include "Systems/InputSystem.hpp"
#include "Systems/GraphicsSystem.hpp"
#include "Systems/PhysicsSystem.hpp"
#include "Systems/EventSystem.hpp"
#include "Systems/SaveSystem.hpp"
#include "Systems/ConfigSystem.hpp"
#include "Systems/SoundSystem.hpp"
#include "Systems/Logger.hpp"
#include "Scene/SceneManager.hpp"
#include "Scene/Scene.hpp"
#include "Interfaces/ITimeProvider.hpp"
#include "Interfaces/IInputProvider.hpp"
#include "Interfaces/IGraphics.hpp"
#include "Interfaces/IPhysicsSystem.hpp"
#include "Interfaces/ISceneManager.hpp"
#include "EngineConfig.hpp"

namespace GameLibrary
{
    bool Engine::Init(HINSTANCE hInstance, const wchar_t* title, const char* configPath)
    {
        // config 경로 저장
        if (configPath)
        {
            m_configPath = configPath;
        }

        // 서비스 등록
        m_container.Register<EngineConfig, EngineConfig>();
        m_container.Register<ConfigSystem, ConfigSystem>();
        m_container.Register<Window, Window>();
        m_container.Register<ITimeProvider, TimeSystem>();
        m_container.Register<IInputProvider, InputSystem>();
        m_container.Register<IGraphics, GraphicsSystem>();
        m_container.Register<IPhysicsSystem, PhysicsSystem>();
        m_container.Register<ISceneManager, SceneManager>();
        m_container.Register<EventSystem, EventSystem>();
        m_container.Register<SaveSystem, SaveSystem>();
        m_container.Register<SoundSystem, SoundSystem>();

        // ConfigSystem 먼저 초기화
        m_configSystem = m_container.Resolve<ConfigSystem>();
        if (!m_configSystem)
        {
            return false;
        }

        // config 파일 로드 (파일 없으면 경고 후 기본값 사용)
        if (!m_configPath.empty())
        {
            if (!m_configSystem->LoadFromFile(m_configPath))
            {
                Logger::Warning("Failed to load config: " + m_configPath);
            }
        }

        // EngineConfig 적용
        auto* config = m_container.Resolve<EngineConfig>();
        if (!config)
        {
            return false;
        }

        config->screenWidth = m_configSystem->GetInt("screenWidth", config->screenWidth);
        config->screenHeight = m_configSystem->GetInt("screenHeight", config->screenHeight);

        // 나머지 서비스 초기화
        m_window = m_container.Resolve<Window>();
        m_timeSystem = static_cast<TimeSystem*>(m_container.Resolve<ITimeProvider>());
        m_inputSystem = static_cast<InputSystem*>(m_container.Resolve<IInputProvider>());
        m_graphicsSystem = static_cast<GraphicsSystem*>(m_container.Resolve<IGraphics>());
        m_sceneManager = static_cast<SceneManager*>(m_container.Resolve<ISceneManager>());
        m_physicsSystem = m_container.Resolve<IPhysicsSystem>();
        m_eventSystem = m_container.Resolve<EventSystem>();
        m_saveSystem = m_container.Resolve<SaveSystem>();
        m_soundSystem = m_container.Resolve<SoundSystem>();

        if (!m_window || !m_timeSystem || !m_inputSystem || !m_graphicsSystem || !m_sceneManager || !m_eventSystem || !m_saveSystem || !m_soundSystem)
        {
            return false;
        }

        if (!m_window->Create(hInstance, config->screenWidth, config->screenHeight, title))
        {
            return false;
        }

        if (!m_graphicsSystem->Init(m_window->GetHandle(), config->screenWidth, config->screenHeight))
        {
            return false;
        }

        m_sceneManager->SetContainer(&m_container);

        m_isRunning = true;
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

        if (m_graphicsSystem)
        {
            m_graphicsSystem->Shutdown();
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

        if (!m_configPath.empty())
        {
            if (!m_configSystem->LoadFromFile(m_configPath))
            {
                Logger::Warning("Failed to reload config: " + m_configPath);
            }
        }

        // 참고: screenWidth/screenHeight는 Window 재생성이 필요해서 런타임에 적용 안됨
    }

    void Engine::Update()
    {
        m_timeSystem->Update();
        m_inputSystem->Update();

        ProcessGlobalInput();

        float deltaTime = m_timeSystem->GetDeltaTime();
        m_sceneManager->Update(deltaTime);

        if (m_physicsSystem)
        {
            m_physicsSystem->CheckCollisions();
        }
    }

    void Engine::Render()
    {
        m_graphicsSystem->Clear({ 0, 0, 0, 255 });
        m_sceneManager->Render(*m_graphicsSystem);
        m_graphicsSystem->Present();
    }

    void Engine::ProcessGlobalInput()
    {
        // F5: Config 리로드 후 현재 씬 재시작
        if (m_inputSystem->IsKeyPressed(KeyCode::F5))
        {
            ReloadConfig();

            if (auto* currentScene = m_sceneManager->GetCurrentScene())
            {
                m_sceneManager->LoadScene(currentScene->GetName());
            }
        }
    }
}