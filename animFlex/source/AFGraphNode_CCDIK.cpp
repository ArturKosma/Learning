#include "AFGraphNode_CCDIK.h"

#include "AFMath.h"

void AFGraphNode_CCDIK::Evaluate(float deltaTime)
{
	m_outputPose.SetValue(m_inputPose.GetValue());

	// Safe division.
	const float blendInTime = AFMath::NearlyEqual(m_blendInTime.GetValue(), 0.0f) ? 0.0f : 1.0f / m_blendInTime.GetValue();
	const float blendOutTime = AFMath::NearlyEqual(m_blendOutTime.GetValue(), 0.0f) ? 0.0f : 1.0f / m_blendOutTime.GetValue();

	// Blend the alpha.
	m_currentAlpha = AFMath::FInterpToConst(m_currentAlpha,
		m_active ? 1.0f : 0.0f, 
		m_active ? blendInTime : blendOutTime,
		deltaTime);

	printf("%f\n", m_currentAlpha);
}
