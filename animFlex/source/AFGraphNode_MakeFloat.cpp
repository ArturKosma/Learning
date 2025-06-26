#include "AFGraphNode_MakeFloat.h"

void AFGraphNode_MakeFloat::Evaluate(float deltaTime)
{
	Output.SetValue(Input.GetValue());
}
