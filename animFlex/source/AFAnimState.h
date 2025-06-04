#pragma once
#include <memory>
#include <stdint.h>

class AFAnimationClip;
class AFSkeletalMeshComponent;

enum class EAFAnimSourceState : uint8_t
{
	SingleAnim,
	Graph
};

enum class EAFAnimEvaluationState : uint8_t
{
	Idle,
	Playing
};

class AFAnimState
{
public:

	void Tick(float deltaTime);

	void SetOwnerMesh(AFSkeletalMeshComponent* newOwner);

	void SetAnimation(std::shared_ptr<AFAnimationClip> animation);
	void SetEvaluationState(EAFAnimEvaluationState newEvaluationState);

private:

	void EvaluateSingleAnim();

	EAFAnimSourceState m_sourceState = EAFAnimSourceState::Graph;
	EAFAnimEvaluationState m_evaluationState = EAFAnimEvaluationState::Playing;
	AFSkeletalMeshComponent* m_ownerMesh = nullptr;

	std::shared_ptr<AFAnimationClip> m_singleAnim = nullptr;
};