#pragma once

namespace GameLibrary
{
    class ITimeProvider
    {
    public:
        virtual ~ITimeProvider() = default;

        virtual void Update() = 0;

        [[nodiscard]] virtual float GetDeltaTime() const = 0;
        [[nodiscard]] virtual float GetElapsedTime() const = 0;
    };
} // namespace GameLibrary