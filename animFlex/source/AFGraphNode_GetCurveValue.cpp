#include "AFGraphNode_GetCurveValue.h"

#include "AFAnimState.h"
#include "AFGame.h"
#include "AFPlayerPawn.h"

void AFGraphNode_GetCurveValue::Init()
{
	m_animState = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent()->GetAnimState();
}

void AFGraphNode_GetCurveValue::Evaluate(float deltaTime)
{
	if (!m_animState)
	{
		return;
	}

	m_curveValue.SetValue(m_animState->GetCurveValue(m_curveName.GetValue()));
}
