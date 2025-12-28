#pragma once

namespace GameLibrary
{
    class ICollidable;

    class IPhysicsSystem
    {
    public:
        virtual ~IPhysicsSystem() = default;

        virtual void RegisterCollider(ICollidable* collider) = 0;
        virtual void UnregisterCollider(ICollidable* collider) = 0;
        virtual void CheckCollisions() = 0;
    };
}