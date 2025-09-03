#include "AFGraphNode_Compare.h"

#include "AFMath.h"

void AFGraphNode_Compare::Evaluate(float deltaTime)
{
	bool res = false;

	switch (static_cast<EAFCompareTypes>(m_compareType.GetValue()))
	{
		case EAFCompareTypes::Greater:
		{
			res = m_a.GetValue() > m_b.GetValue();
			break;
		}
		case EAFCompareTypes::GreaterEqual:
		{
			res = m_a >= m_b;
			break;
		}
		case EAFCompareTypes::Equal:
		{
			res = AFMath::NearlyEqual(m_a, m_b);
			break;
		}
		case EAFCompareTypes::NonEqual:
		{
			res = !AFMath::NearlyEqual(m_a, m_b);
			break;
		}
		case EAFCompareTypes::Lesser:
		{
			res = m_a < m_b;
			break;
		}
		case EAFCompareTypes::LesserEqual:
		{
			res = m_a <= m_b;
			break;
		}
	}

	m_compareResult.SetValue(res);
}
