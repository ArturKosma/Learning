#include "ControlRigCustomNodes.h"

#include "Kismet/KismetMathLibrary.h"

FRigUnit_QuatTwistAngle_Execute()
{
	// Don't divide by zero.
	if (TwistAxis.Length() < 1e-8)
	{
		return;
	}

	// Normalize twist axis.
	const FVector twistAxis = TwistAxis.GetSafeNormal();

	// Project rotation axis onto twist axis.
	const FVector qAxis = FVector(Quat.X, Quat.Y, Quat.Z);
	const FVector proj = twistAxis * UKismetMathLibrary::Dot_VectorVector(qAxis, twistAxis);

	// Build twist quaternion.
	FQuat twist = FQuat(proj.X, proj.Y, proj.Z, Quat.W);
	twist = twist.GetNormalized();

	// Get angle.
	const float w = FMath::Max(-1.0f, FMath::Min(1.0f, twist.W));
	float radians = 2.0f * FMath::Acos(w);

	// Signed angle.
	const float sign = FMath::Sign(UKismetMathLibrary::Dot_VectorVector(twistAxis, FVector(twist.X, twist.Y, twist.Z)));
	radians *= sign;

	// Wrap to [-pi, pi].
	const float pi = UKismetMathLibrary::GetPI();
	if (radians > pi)
	{
		radians -= 2.0f * pi;
	}
	else if (radians < -pi)
	{
		radians += 2.0f * pi;
	}

	TwistAngle = FMath::RadiansToDegrees(radians);
}
