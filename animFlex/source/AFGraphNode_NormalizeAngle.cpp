#include "AFGraphNode_NormalizeAngle.h"
#include "AFMath.h"

void AFGraphNode_NormalizeAngle::Evaluate(float deltaTime)
{
	m_result.SetValue(AFMath::NormalizeAngle(m_value.GetValue()));
}
