#include "AFGraphNode_GetCurveFromPose.h"

void AFGraphNode_GetCurveFromPose::Evaluate(float deltaTime)
{
	m_outputPose.SetValue(m_inputPose.GetValue());

	const std::string& curveName = m_curveName.GetValue();
	if (!curveName.empty())
	{
		const auto& curves = m_inputPose.GetValue().GetCurvesValues();
		auto it = curves.find(curveName);
		if (it != curves.end())
		{
			m_curveValue.SetValue(it->second);
		}
	}
}
