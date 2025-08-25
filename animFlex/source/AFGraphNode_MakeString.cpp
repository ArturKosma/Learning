#include "AFGraphNode_MakeString.h"

void AFGraphNode_MakeString::Evaluate(float deltaTime)
{
	Output.SetValue(Input.GetValue());
}