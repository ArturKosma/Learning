#include "AFGraphNode_BoolOperator.h"

void AFGraphNode_BoolOperator::Evaluate(float deltaTime)
{
	bool res = false;

	switch (static_cast<EAFBoolOperators>(m_propertyEnum.GetValue()))
	{
		case EAFBoolOperators::And:
		{
			res = m_a.GetValue() && m_b.GetValue();
			break;
		}
		case EAFBoolOperators::Or:
		{
			res = m_a.GetValue() || m_b.GetValue();
			break;
		}
	}

	m_boolOperatorResult.SetValue(res);
}
