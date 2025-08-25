#include "AFGraphNode_ChooseByAngle.h"

#include "AFMath.h"

void AFGraphNode_ChooseByAngle::Init()
{
	m_posesArray.push_back(&chooseByAngle_m180);
	m_posesArray.push_back(&chooseByAngle_m135);
	m_posesArray.push_back(&chooseByAngle_m90);
	m_posesArray.push_back(&chooseByAngle_m45);
	m_posesArray.push_back(&chooseByAngle_0);
	m_posesArray.push_back(&chooseByAngle_45);
	m_posesArray.push_back(&chooseByAngle_90);
	m_posesArray.push_back(&chooseByAngle_135);
	m_posesArray.push_back(&chooseByAngle_180);
}

void AFGraphNode_ChooseByAngle::Evaluate(float deltaTime)
{
	const float sign = glm::sign(chooseByAngle_angle.GetValue());
	const float absAngle = glm::abs(chooseByAngle_angle.GetValue());
	const float modAngle = glm::mod(absAngle, 360.0f);

	const float angle = sign * modAngle;
	const size_t index = AFMath::NearestIndex<float>(m_anglesArray, angle);

	chooseByAngle_outputPose.SetValue(m_posesArray[index]->GetValue());
}
