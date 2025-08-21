#include "AFGraphNode_GetCurve.h"
#include "AFContent.h"

void AFGraphNode_GetCurve::OnUpdate()
{
	if (m_curveName.GetValue().empty())
	{
		return;
	}

	auto onComplete = [this](std::shared_ptr<AFFloatCurve> fetchedCurve)
		{
			m_curve = fetchedCurve;
		};

	AFContent::Get().FetchAsset<AFFloatCurve>("https://cdn.jsdelivr.net/gh/ArturKosma/assets@main/curves/",
		m_curveName.GetValue() + ".json",
		onComplete);
}

void AFGraphNode_GetCurve::Evaluate(float deltaTime)
{
	if (!m_curve)
	{
		return;
	}

	float ret = 0.0f;

	switch (static_cast<EAFCurveFetchType>(m_fetchType.GetValue()))
	{
		case EAFCurveFetchType::ByTime:
		{
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
