#pragma once
#include <functional>
#include <memory>

#include "AFStructs.h"

using AlphaFunctor = std::function<void(float alpha, float timeElapsed)>;
using AlphaFunctorFin = std::function<void()>;

class AFAlphaTimer
{
	friend class AFTimerManager;

public:

	float GetCurrentAlpha() const;

private:

	void Tick(float deltaTime);

	AlphaFunctor m_alphaFunctor = {};
	AlphaFunctorFin m_alphaFunctorFin = {};

	EAFInterpolationType m_alphaInterpMethod = {};

	float m_timeElapsed = 0.0f;
	float m_alpha = 0.0f;
	float m_length = 0.0f;
	bool m_inverse = false;
};

class AFTimerManager
{
	friend class AFApp;

public:

	static std::shared_ptr<AFAlphaTimer> SetAlphaTimer(const AlphaFunctor& functor, float length, 
		const AlphaFunctorFin& functorFin = {}, EAFInterpolationType interpMethod = EAFInterpolationType::Linear, bool inverse = false);

	static AFTimerManager& GetInstance();
	static float GetDeltaTime();

private:

	void Init();
	void Tick(float deltaTime);

	void DeltaCalc();

	std::vector<std::shared_ptr<AFAlphaTimer>> m_alphaTimers;

	float m_previousTime = 0.0f;
	float m_deltaTime = 0.0f;
};
