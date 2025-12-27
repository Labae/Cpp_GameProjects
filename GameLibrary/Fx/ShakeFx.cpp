#include "ShakeFx.hpp"

#include <random>
#include <chrono>

namespace GameLibrary
{
	ShakeFx::ShakeFx(float intensity, float duration)
		: m_intensity(intensity), m_duration(duration)
	{
	}

	void ShakeFx::Update(float deltaTime)
	{
		m_elapsed += deltaTime;

		if (m_elapsed >= m_duration)
		{
			m_finished = true;
			m_offsetX = 0;
			m_offsetY = 0;
			return;
		}

		float ratio = 1.0f - (m_elapsed / m_duration);
		float currentIntensity = m_intensity * ratio;

		auto seed = static_cast<unsigned>(
			std::chrono::high_resolution_clock::now().time_since_epoch().count()
			);
		std::mt19937 gen(seed);
		std::uniform_real_distribution<float> dist(-currentIntensity, currentIntensity);
		
		m_offsetX = static_cast<int32_t>(dist(gen));
		m_offsetY = static_cast<int32_t>(dist(gen));
	}

	void ShakeFx::Render(IGraphics& graphics)
	{
	}
}