#include "AFGraphNode_Blend.h"

AFGraphNode_Blend::AFGraphNode_Blend()
{
}

void AFGraphNode_Blend::Evaluate(float deltaTime)
{
	printf("eval Blend, alpha: %f\n", alpha.GetValue());
}
