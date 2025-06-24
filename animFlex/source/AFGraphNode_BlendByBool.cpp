#include "AFGraphNode_BlendByBool.h"

void AFGraphNode_BlendByBool::Evaluate(float deltaTime)
{
	if (useA.GetValue())
	{
		printf("evaluating blend by bool: true\n");
	}
	else
	{
		printf("evaluating blend by bool: false\n");
	}
}
