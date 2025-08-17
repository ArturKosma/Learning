#include "AFGraphNode_BoolNot.h"

void AFGraphNode_BoolNot::Evaluate(float deltaTime)
{
	m_boolNotResult.SetValue(!m_boolNotInput.GetValue());
}
