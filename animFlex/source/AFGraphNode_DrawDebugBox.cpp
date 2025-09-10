#include "AFGraphNode_DrawDebugBox.h"

#include "AFMath.h"
#include "AFUtility.h"

void AFGraphNode_DrawDebugBox::Evaluate(float deltaTime)
{
	AFPose& pose = const_cast<AFPose&>(m_inputPose.GetValue());

	AFUtility::DrawDebugBox(m_location.GetValue(),
		m_size.GetValue(),
		m_lifetime,
		static_cast<EAFColor>(m_color.GetValue()),
		AFMath::QuaternionFromEuler(m_rotation.GetValue()));

	m_outputPose.SetValue(pose);
}
