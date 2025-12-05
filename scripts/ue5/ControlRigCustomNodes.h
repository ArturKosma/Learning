#pragma once

#include "Units/RigUnit.h"
#include "ControlRigCustomNodes.generated.h"

USTRUCT(meta = (DisplayName = "Quat To Twist Angle"), Category = "CustomNodes")
struct CPPLEARN_API FRigUnit_QuatTwistAngle : public FRigUnit
{
	GENERATED_BODY()

	FRigUnit_QuatTwistAngle() :
		Quat(FQuat::Identity), TwistAxis(FVector(1.0f, 0.0f, 0.0f)), TwistAngle(0.0f)
	{
		
	}

	RIGVM_METHOD()
	virtual void Execute() override;

	UPROPERTY(meta = (Input))
	FQuat Quat;

	UPROPERTY(meta = (Input))
	FVector TwistAxis;

	UPROPERTY(meta = (Output))
	float TwistAngle;
};
