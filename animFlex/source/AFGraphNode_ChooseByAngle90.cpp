#include "AFGraphNode_ChooseByAngle90.h"

#include "AFMath.h"

void AFGraphNode_ChooseByAngle90::Init()
{
	m_posesArray.push_back(&chooseByAngle_m180);
	m_posesArray.push_back(&chooseByAngle_m90);
	m_posesArray.push_back(&chooseByAngle_0);
	m_posesArray.push_back(&chooseByAngle_90);
	m_posesArray.push_back(&chooseByAngle_180);
}

void AFGraphNode_ChooseByAngle90::Evaluate(float deltaTime)
{
	const float sign = glm::sign(chooseByAngle_angle.GetValue());
	const float absAngle = glm::abs(chooseByAngle_angle.GetValue());
	const float modAngle = glm::mod(absAngle, 360.0f);

	const float angle = sign * modAngle;
	m_chosenIndex = AFMath::NearestIndex<float>(m_anglesArray, angle);

	chooseByAngle_outputPose.SetValue(m_posesArray[m_chosenIndex]->GetValue());
}
