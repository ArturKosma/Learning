#include "AFGraphNode_Print.h"

void AFGraphNode_Print::Evaluate(float deltaTime)
{
	if (m_printFloat)
	{
		printf("%f\n", m_inputFloat.GetValue());
	}
	if (m_printString)
	{
		printf("%s\n", m_inputString.GetValue().c_str());
	}

	m_outputPose.SetValue(m_inputPose.GetValue());
}
