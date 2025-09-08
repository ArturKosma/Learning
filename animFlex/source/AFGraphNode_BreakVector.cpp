#include "AFGraphNode_BreakVector.h"

void AFGraphNode_BreakVector::Evaluate(float deltaTime)
{
	const glm::vec3& input = m_inputVector.GetValue();

	m_x.SetValue(input.x);
	m_y.SetValue(input.y);
	m_z.SetValue(input.z);
}
