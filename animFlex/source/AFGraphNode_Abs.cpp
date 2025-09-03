#include "AFGraphNode_Abs.h"

void AFGraphNode_Abs::Evaluate(float deltaTime)
{
	m_output.SetValue(glm::abs(m_input.GetValue()));
}
