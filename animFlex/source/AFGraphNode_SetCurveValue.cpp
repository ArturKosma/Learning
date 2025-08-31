#include "AFGraphNode_SetCurveValue.h"

void AFGraphNode_SetCurveValue::Evaluate(float deltaTime)
{
	const AFPose& pose = m_inputPose.GetValue();

	std::string curveName = "";

	switch (m_curveEnum.GetValue())
	{
		case 0: { curveName = "rootDistance"; break; }
		case 1: { curveName = "rootYaw"; break; }
		case 2: { curveName = "rootYawDelta"; break; }
		case 3: { curveName = "rootYawAuthority"; break; }
		case 4: { curveName = "rootYawWarpScale"; break; }
		case 5: { curveName = "rootYawLeft"; break; }
		default: { break; }
	}

	if (!curveName.empty())
	{
		std::unordered_map<std::string, float>& curves = const_cast<std::unordered_map<std::string, float>&>(pose.GetCurvesValues());
		auto it = curves.find(curveName);

		if (it != curves.end())
		{
			curves[curveName] = m_inputValue.GetValue();
		}
	}

	m_outputPose.SetValue(pose);
}
