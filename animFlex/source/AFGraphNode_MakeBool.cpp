#include "AFGraphNode_MakeBool.h"

void AFGraphNode_MakeBool::Evaluate(float deltaTime)
{
	Output.SetValue(Input.GetValue());
}
