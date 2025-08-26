#include "AFGraphNode_GetCurve.h"
#include "AFContent.h"

void AFGraphNode_GetCurve::OnUpdate()
{
	const std::string& curveName = m_curveName.GetValue();

	if (curveName.empty())
	{
		m_curve = nullptr;
		return;
	}

	if (m_curve && m_curve->GetName() == curveName)
	{
		return;
	}

	auto onComplete = [this](std::shared_ptr<AFFloatCurve> fetchedCurve)
		{
			m_curve = fetchedCurve;
		};

	AFContent::Get().FetchAsset<AFFloatCurve>("https://cdn.jsdelivr.net/gh/ArturKosma/assets@main/curves/",
		m_curveName.GetValue(),
		onComplete, ".json");
}

void AFGraphNode_GetCurve::Evaluate(float deltaTime)
{
	OnUpdate();

	if (!m_curve)
	{
		m_curveValue.SetValue(0.0f);
		return;
	}

	float ret = 0.0f;

	switch (static_cast<EAFCurveFetchType>(m_fetchType.GetValue()))
	{
		case EAFCurveFetchType::ByTime:
		{
			// Takes in a time and returns value for it.
			ret = m_curve->SampleByTime(m_inputValue);
			break;
		}
		case EAFCurveFetchType::ByValue:
		{
			// Takes in a value and returns time for it.
			ret = m_curve->SampleByValue(m_inputValue);
			break;
		}
		default:
		{
			ret = 0.0f;
			break;
		}
	}

	m_curveValue.SetValue(ret);
}
