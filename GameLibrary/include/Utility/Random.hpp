#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <algorithm>
#include <random>
#include <vector>

namespace GameLibrary
{
    class Random
    {
    public:
        Random() = delete;

        [[nodiscard]] static int32_t Range(int32_t min, int32_t max)
        {
            std::uniform_int_distribution<int32_t> dist(min, max);
            return dist(GetEngine());
        }

        [[nodiscard]] static float Range(float min, float max)
        {
            std::uniform_real_distribution<float> dist(min, max);
            return dist(GetEngine());
        }

        [[nodiscard]] static bool Chance(const float probability)
        {
            return Range(0.0f, 1.0f) < probability;
        }

        template <typename T>
        static void Shuffle(std::vector<T>& container)
        {
            std::ranges::shuffle(container, GetEngine());
        }

        template <typename T>
        [[nodiscard]] static const T& Pick(const std::vector<T>& container)
        {
            const auto index = Range(0, static_cast<int32_t>(container.size()) - 1);
            return container[static_cast<size_t>(index)];
        }

        static void SetSeed(const uint32_t seed)
        {
            GetEngine().seed(seed);
        }

    private:
        [[nodiscard]] static std::mt19937& GetEngine()
        {
            static std::mt19937 engine{std::random_device{}()};
            return engine;
        }
    };
} // namespace GameLibrary

#endif // RANDOM_HPP