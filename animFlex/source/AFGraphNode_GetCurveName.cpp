#include "AFGraphNode_GetCurveName.h"

#include "AFContent.h"

void AFGraphNode_GetCurveName::OnUpdate()
{
	const std::string& curveName = Input.GetValue();

	if (curveName.empty())
	{
		return;
	}

	auto onComplete = [](std::shared_ptr<AFFloatCurve> fetchedCurve)
		{
			
		};

	AFContent::Get().FetchAsset<AFFloatCurve>("https://cdn.jsdelivr.net/gh/ArturKosma/assets@main/curves/",
		curveName,
		onComplete, ".json");
}

void AFGraphNode_GetCurveName::Evaluate(float deltaTime)
{
	Output.SetValue(Input.GetValue());
}
