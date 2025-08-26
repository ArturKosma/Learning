#include "AFGraphNode_Clamp.h"

void AFGraphNode_Clamp::Evaluate(float deltaTime)
{
	m_result.SetValue(glm::clamp(m_value.GetValue(), m_min.GetValue(), m_max.GetValue()));
}
