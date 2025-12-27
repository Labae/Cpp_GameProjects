#pragma once

namespace GameLibrary
{
    class ITimeProvider
    {
    public:
        virtual ~ITimeProvider() = default;

        virtual float GetDeltaTime() const = 0;
        virtual float GetElapsedTime() const = 0;
    };
}