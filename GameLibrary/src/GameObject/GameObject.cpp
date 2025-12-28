#include "GameObject/GameObject.hpp"

#include "GameObject/Transform.hpp"

namespace GameLibrary
{
    GameObject::GameObject() : m_isActive(true)
    {
        m_transform = AddComponent<Transform>();
    }

    void GameObject::Init() {}

    void GameObject::Destroy()
    {
        for (auto& component : m_components)
        {
            component->Destroy();
        }
    }

    void GameObject::Update(float deltaTime)
    {
        for (auto& component : m_components)
        {
            component->Update(deltaTime);
        }
    }

    void GameObject::Render(IGraphics& graphics)
    {
        for (auto& component : m_components)
        {
            component->Render(graphics);
        }
    }
} // namespace GameLibrary