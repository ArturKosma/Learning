#include "AFGraphNode_FloatOperation.h"

void AFGraphNode_FloatOperation::Evaluate(float deltaTime)
{
	float ret = 0.0f;

	switch (static_cast<EAFFloatOperations>(m_operation.GetValue()))
	{
		case EAFFloatOperations::Add:
		{
			ret = m_a + m_b;
			break;
		}
		case EAFFloatOperations::Substract:
		{
			ret = m_a - m_b;
			break;
		}
		case EAFFloatOperations::Multiply:
		{
			ret = m_a * m_b;
			break;
		}
		case EAFFloatOperations::Divide:
		{
			float b = m_b.GetValue();
			b = glm::sign(b == 0.0f ? 1.0f : b) * glm::max(glm::abs(b), glm::epsilon<float>());

			ret = m_a / b;
			break;
		}
		case EAFFloatOperations::Modulo:
		{
			ret = glm::mod(m_a.GetValue(), m_b.GetValue());
			break;
		}
	}

	m_result.SetValue(ret);
}
