#include "AFGraphNode_MakeVector.h"

void AFGraphNode_MakeVector::Evaluate(float deltaTime)
{
	m_outputVector.SetValue({ m_x.GetValue(), m_y.GetValue(), m_z.GetValue() });
}
