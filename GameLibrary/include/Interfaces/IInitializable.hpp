#pragma once

namespace GameLibrary
{
    class ServiceContainer;

    class IInitializable
    {
    public:
        virtual ~IInitializable() = default;
        virtual void Init(ServiceContainer& container) = 0;
    };
} // namespace GameLibrary