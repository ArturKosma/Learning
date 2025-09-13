#include "AFGraphNode_GetAnimName.h"

#include "AFContent.h"

void AFGraphNode_GetAnimName::OnUpdate()
{
	const std::string& animName = Input.GetValue();

	// No anim name.
	if (animName.empty())
	{
		return;
	}

	/*auto onCompleteCurves = [](std::vector<std::shared_ptr<AFFloatCurve>> fetchedCurves)
		{

		};*/

	// Called upon anims fetch complete.
	auto onComplete = [this, /*onCompleteCurves,*/ animName](std::shared_ptr<AFAnimationClip> fetchedAnim)
		{
			m_animClip = fetchedAnim;

			// Fetch curves.
			/*AFContent::Get().FetchAssets<AFFloatCurve>("content/curves/manifest.json",
				"https://cdn.jsdelivr.net/gh/ArturKosma/assets@main/curves/",
				animName,
				onCompleteCurves, ".json");*/
		};

	// Fetch anim.
	AFContent::Get().FetchAsset<AFAnimationClip>("https://cdn.jsdelivr.net/gh/ArturKosma/assets@main/anims/",
		animName,
		onComplete, ".afanim");
}

void AFGraphNode_GetAnimName::Evaluate(float deltaTime)
{
	Output.SetValue(Input.GetValue());
}
