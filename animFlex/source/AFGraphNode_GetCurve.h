#pragma once
#include "AFGraphNode.h"

AFENUM()
enum class EAFCurveFetchType
{
	ByTime,
	ByValue
};

class AFGraphNode_GetCurve : public AFGraphNodeCRTP<AFGraphNode_GetCurve>
{
	AFCLASS(AFGraphNode_GetCurve, "Get Curve", "")

public:

	AFPARAM(std::string, m_curveName, "", "", "Input", "HidePin|Dropdown_Curves");
	AFPARAM(int, m_fetchType, 0, "Fetch Type", "Input", "HidePin|EAFCurveFetchType_Enum");
	AFPARAM(float, m_inputValue, 0.0f, "", "Input", "");
	AFPARAM(float, m_curveValue, 0.0f, "Output", "Output", "");

	void OnUpdate() override;
	void Evaluate(float deltaTime) override;

private:

	std::shared_ptr<AFFloatCurve> m_curve = nullptr;
};
