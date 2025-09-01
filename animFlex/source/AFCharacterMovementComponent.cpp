#include "AFCharacterMovementComponent.h"
#include "AFActor.h"
#include "AFAnimState.h"
#include "AFEvaluator.h"
#include "AFMath.h"
#include "AFPlayerPawn.h"

float AFCharacterMovementComponent::GetAcceleration() const
{
	return 800.0f;
}

float AFCharacterMovementComponent::GetDeceleration() const
{
	return 1500.0f;
}

float AFCharacterMovementComponent::GetMaxSpeed() const
{
	return 500.0f;
}

void AFCharacterMovementComponent::Tick(float deltaTime)
{
	const bool paused = AFEvaluator::Get().GetAnimPaused();
	const float playrate = AFEvaluator::Get().GetAnimPlayrate();
	deltaTime = paused ? 0.0f : deltaTime * playrate;

	AFMovementComponent::Tick(deltaTime);

	std::shared_ptr<AFPlayerPawn> ownerPawn = std::dynamic_pointer_cast<AFPlayerPawn>(GetOwner().lock());
	if (ownerPawn)
	{
		if (glm::length(m_velocity) <= std::numeric_limits<float>::epsilon())
		{
			m_previousRootYaw = 0.0f;
			return;
		}

		std::shared_ptr<AFAnimState> animState = ownerPawn->GetMeshComponent()->GetAnimState();
		if (!animState)
		{
			return;
		}

		m_distanceTraveled += glm::length(GetLastLocationOffset());

		// Rotation from rootYaw, which acts like rootMotion for rotation.
		//const float rootYaw = animState->GetCurveValue("rootYaw");
		const float rootYaw = 0.0f;
		//printf("%s: %f\n", "rootYaw", rootYaw);
		//printf("%s: %f\n", "previousRootYaw", m_previousRootYaw);

		// Root yaw delta.
		const float rootYawDelta = rootYaw - m_previousRootYaw;
		m_previousRootYaw = rootYaw;

		// How much yaw is left in the animation.
		const float rootYawLeft = animState->GetCurveValue("rootYawLeft");
		const float rootYawLeftSafe = glm::max(glm::abs(rootYawLeft), glm::epsilon<float>()) * glm::sign(rootYawLeft);

		// How much more we need towards target yaw.
		const glm::quat& actorWorldRotation = ownerPawn->GetRotationQuat();
		const glm::vec3& actorWorldForward = glm::normalize(actorWorldRotation * glm::vec3(0.0, 0.0f, 1.0f));
		const float yawLeftCharacter = AFMath::SignedAngleBetweenVectors(m_lastPositiveMovementInput, actorWorldForward, glm::vec3(0.0f, 1.0f, 0.0f));

		// Yaw delta warp factor.
		const float k = glm::clamp(yawLeftCharacter / rootYawLeftSafe, 0.0f, 5.0f);

		//printf("%s: %f\n", "char left", rootYawLeftCharacter);
		//printf("%s: %f\n", "anim left", rootYawLeftSafe);
		//printf("%s: %f\n", "k", k);
		//printf("%s: %f\n", "rootYawDelta", rootYawDelta);

		//const float rootYawDeltaScaled = rootYawDelta * rootYawWarpScale;
		//float scaledDiff = glm::abs(glm::abs(rootYawDelta) - glm::abs(rootYawDeltaScaled));
		//const float scaledDiffBias = 0.4f;
		//scaledDiff = AFMath::MapRangeClamped(scaledDiff, scaledDiffBias, 0.8f, 0.0f, 1.0f);

		//printf("%s: %f\n", "raw delta", rootYawDelta);
		//printf("%s: %f\n", "scaled delta", rootYawDeltaScaled);

		// Scales how much we use mechanical rotation towards last velocity vs curve from animation.
		const float rootYawAuthority = animState->GetCurveValue("rootYawAuthority");
		float rotateSpeedTowardsVel = 250.0f * (1.0f - rootYawAuthority);

		// How much should we use the pivoted location?
		float pivotedScale = 0.0f;

		// Find delta yaw, either from curve or mechanically.
		float deltaYaw = 0.0f;
		if (glm::abs(rootYawDelta) > glm::epsilon<float>())
		{
			deltaYaw = rootYawDelta * k;
			//pivotedScale = glm::abs(1.0f - glm::clamp(rootYawWarpScale, 0.4f, 1.6f));
			//pivotedScale = AFMath::MapRangeClamped(pivotedScale, 0.0f, 0.6f, 0.0f, 1.0f);
		}
		else if (rotateSpeedTowardsVel > glm::epsilon<float>())
		{
			const glm::vec3 up(0.0f, 1.0f, 0.0f);
			const glm::vec3 forward(0.0f, 0.0f, 1.0f);

			const glm::quat curRot = ownerPawn->GetRotationQuat();
			const glm::vec3 curFwd = glm::normalize(glm::rotate(curRot, forward));
			const glm::vec3 tgtFwd = AFMath::GetSafeNormal(m_velocity);

			const float rad = std::atan2(glm::dot(up, glm::cross(curFwd, tgtFwd)),
				glm::dot(curFwd, tgtFwd));
			float deltaYawDeg = glm::degrees(rad);

			const float maxStep = rotateSpeedTowardsVel * deltaTime;
			deltaYawDeg = glm::clamp(deltaYawDeg, -maxStep, maxStep);

			if (glm::abs(deltaYawDeg) > glm::epsilon<float>())
			{
				deltaYaw = deltaYawDeg;
			}
		}

		// Find location after rotating around possible pivot.
		const glm::vec3 pivotedLoc = GetLocationAfterBallPivotRot(deltaYaw);

		// How much should we use the pivoted location?
		//printf("%f\n", deltaYaw);
		const glm::vec3 finalLoc = glm::mix(ownerPawn->GetLocation(), pivotedLoc, pivotedScale);

		// Add rotation delta.
		ownerPawn->AddOffsetRotation(glm::vec3(0.0f, deltaYaw, 0.0f));

		// Set pivoted location.
		ownerPawn->SetLocation(finalLoc);
	}
}

float AFCharacterMovementComponent::GetDistanceTraveled() const
{
	return m_distanceTraveled;
}

void AFCharacterMovementComponent::ResetDistanceTraveled()
{
	m_distanceTraveled = 0.0f;
}

glm::vec3 AFCharacterMovementComponent::GetLocationAfterBallPivotRot(float deltaYaw)
{
	std::shared_ptr<AFPlayerPawn> ownerPawn = std::dynamic_pointer_cast<AFPlayerPawn>(GetOwner().lock());
	if (!ownerPawn)
	{
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}

	// Ball locations.
	const glm::vec3 lBallLoc = ownerPawn->GetMeshComponent()->GetJointLocation(72);
	const glm::vec3 rBallLoc = ownerPawn->GetMeshComponent()->GetJointLocation(79);

	// Which feet is planted.
	const bool lPlanted = lBallLoc.y < 5.0f;
	const bool rPlanted = rBallLoc.y < 5.0f;

	// Get ball locations.
	glm::vec3 lBallLocation = ownerPawn->GetMeshComponent()->GetJointLocation(72);
	glm::vec3 rBallLocation = ownerPawn->GetMeshComponent()->GetJointLocation(79);

	glm::vec3 pivotOffset = glm::vec3(0.0f, 0.0f, 0.0f);
	if (lPlanted && rPlanted)
	{
		const float sign = glm::sign(lBallLocation.y - rBallLocation.y);
		const float diff = glm::abs(lBallLocation.y - rBallLocation.y);
		if (diff > 0.1f) // If both are planted there has to be a visible difference.
		{
			if (sign > 0.0f)
			{
				//printf("L planted\n");
				pivotOffset = lBallLocation;
			}
			else
			{
				//printf("R planted\n");
				pivotOffset = rBallLocation;
			}
		}
		else // If both are at same height we pick one of them.
		{
			//printf("L planted\n");
			pivotOffset = lBallLocation;
		}
	}
	else if (lPlanted)
	{
		//printf("L planted\n");
		pivotOffset = lBallLocation;
	}
	else if (rPlanted)
	{
		//printf("R planted\n");
		pivotOffset = rBallLocation;
	}
	else
	{
		//printf("nothing planted\n");
	}

	const glm::mat4 m = ownerPawn->GetTransform();
	const glm::vec3 pivotWorld = glm::vec3(m * glm::vec4(pivotOffset, 1.0f));

	const glm::quat deltaQuat = glm::quat(glm::radians(glm::vec3(0.0f, deltaYaw, 0.0f)));
	const glm::mat4 r = glm::mat4_cast(deltaQuat);

	const glm::mat4 tPos = glm::translate(glm::mat4(1.0f), pivotWorld);
	const glm::mat4 tNeg = glm::translate(glm::mat4(1.0f), -pivotWorld);

	const glm::mat4 out = glm::mat4(tPos * r * tNeg * m);

	glm::vec3 scale;
	glm::quat orientation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(out, scale, orientation, translation, skew, perspective);

	return translation;
}
