#include "AFTimerManager.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>
#include <algorithm>

#include "AFStructs.h"
#include "AFMath.h"

float AFAlphaTimer::GetCurrentAlpha() const
{
	return m_alpha;
}

void AFAlphaTimer::Tick(float deltaTime)
{
	m_timeElapsed += deltaTime;
	m_timeElapsed = glm::clamp(m_timeElapsed, 0.0f, m_length);

	m_alpha = AFMath::MapRangeClamped(m_timeElapsed, 0.0f, m_length, 0.0f, 1.0f);

	float interpAlpha = m_alpha;

	if(m_alphaInterpMethod == EAFInterpolationType::CubicHermite)
	{
		interpAlpha = (-2.0f * glm::pow(m_alpha, 3.0f)) + (3.0f * glm::pow(m_alpha, 2.0f));
	}

	m_alphaFunctor(m_inverse ? 1.0f - interpAlpha : interpAlpha, m_timeElapsed);
}

std::shared_ptr<AFAlphaTimer> AFTimerManager::SetAlphaTimer(const AlphaFunctor& functor, float length, 
	const AlphaFunctorFin& functorFin, EAFInterpolationType interpMethod, bool inverse)
{
	std::shared_ptr<AFAlphaTimer> newAlphaTimer = std::make_shared<AFAlphaTimer>();
	newAlphaTimer->m_alphaFunctor = functor;
	newAlphaTimer->m_alphaFunctorFin = functorFin;
	newAlphaTimer->m_length = length;
	newAlphaTimer->m_alphaInterpMethod = interpMethod;
	newAlphaTimer->m_inverse = inverse;

	GetInstance().m_alphaTimers.push_back(newAlphaTimer);

	newAlphaTimer->Tick(0.0f);

	return newAlphaTimer;
}

AFTimerManager& AFTimerManager::GetInstance()
{
	static AFTimerManager timerManager;
	return timerManager;
}

float AFTimerManager::GetDeltaTime()
{
	return GetInstance().m_deltaTime;
}

void AFTimerManager::Init()
{
	m_previousTime = static_cast<float>(glfwGetTime());
}

void AFTimerManager::Tick(float deltaTime)
{
	for(std::shared_ptr<AFAlphaTimer> alphaTimer : m_alphaTimers)
	{
		alphaTimer->Tick(deltaTime);

		if(alphaTimer->GetCurrentAlpha() == 1.0f)
		{
			if(alphaTimer->m_alphaFunctorFin)
			{
				alphaTimer->m_alphaFunctorFin();
			}
		}
	}

	m_alphaTimers.erase(std::remove_if(m_alphaTimers.begin(), m_alphaTimers.end(),
		[](const std::shared_ptr<AFAlphaTimer>& alphaTimer)
		{
			return alphaTimer->GetCurrentAlpha() == 1.0f;
		}), m_alphaTimers.end());
}

void AFTimerManager::DeltaCalc()
{
	const float currentTime = static_cast<float>(glfwGetTime());
	m_deltaTime = currentTime - m_previousTime;
	m_previousTime = currentTime;
}

void AFTimerManager::ClearTimer(std::shared_ptr<AFAlphaTimer> timer)
{
	std::shared_ptr<AFAlphaTimer> timerToClear = nullptr;

	for (std::shared_ptr<AFAlphaTimer> alphaTimer : m_alphaTimers)
	{
		if (alphaTimer == timer)
		{
			timerToClear = alphaTimer;
		}
	}

	if (!timerToClear)
	{
		return;
	}

	m_alphaTimers.erase(std::remove_if(m_alphaTimers.begin(), m_alphaTimers.end(),
		[timerToClear](const std::shared_ptr<AFAlphaTimer>& alphaTimer)
		{
			return alphaTimer == timerToClear;
		}), m_alphaTimers.end());
}
