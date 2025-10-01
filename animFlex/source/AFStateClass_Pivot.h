#pragma once
#include <memory>
#include "AFFloatCurve.h"
#include "AFStateClass.h"

struct FAFPivotData
{
	std::string anim = "";
	float startTime = 0.0f;
};

class AFStateClass_Pivot : public AFStateClass
{
public:

	bool CallFunctionByString(const std::string& funcName) override;

	std::string GetPivotAnim() const;
	float GetPivotTime() const;
	float GetNewAngleVsAnim() const;

private:

	void OnPivotEnter();
	void OnPivotTick();

	FAFPivotData FindPivotData() const;

	float m_pivotTimeSpent = 0.0f;
	std::string m_pivotAnim = "";
	std::string m_pivotCurve_rootYaw = "";
	std::shared_ptr<AFFloatCurve> m_pivotCurve_rootYawCrv = nullptr;
	float m_pivotStartTime = 0.0f;
	float m_pivotTime = 0.0f;
	float m_previousYawFromCrv = 0.0f;
	float m_newAngleVsAnim = 0.0f;
};