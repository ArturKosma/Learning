#include "AFGraphNode_PlaySequence.h"

void AFGraphNode_PlaySequence::Evaluate(float deltaTime)
{
	printf("eval PlaySequence, anim: %s\n", animName.value.c_str());
}
