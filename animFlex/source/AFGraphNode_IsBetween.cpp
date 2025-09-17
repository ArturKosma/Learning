#include "AFGraphNode_IsBetween.h"

void AFGraphNode_IsBetween::Evaluate(float deltaTime)
{
	const bool l = m_inclusive.GetValue() ? m_a >= m_min : m_a > m_min;
	const bool r = m_inclusive.GetValue() ? m_a <= m_max : m_a < m_max;
	m_out.SetValue(l && r);
}
