#pragma once
#include <memory>
#include <stdint.h>
#include "AFAnimGraph.h"

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
	void SetGraph(std::shared_ptr<AFAnimGraph> animGraph);
	std::shared_ptr<AFAnimGraph> GetGraph() const;

	float GetCurveValue(const std::string& curveName) const;

	void SetEvaluationState(EAFAnimEvaluationState newEvaluationState);

private:

	void EvaluateSingleAnim();
	void EvaluateGraph(float deltaTime);

	EAFAnimSourceState m_sourceState = EAFAnimSourceState::Graph;
	EAFAnimEvaluationState m_evaluationState = EAFAnimEvaluationState::Playing;
	AFSkeletalMeshComponent* m_ownerMesh = nullptr;

	std::shared_ptr<AFAnimationClip> m_singleAnim = nullptr;
	std::shared_ptr<AFAnimGraph> m_graph = nullptr;

	std::unordered_map<std::string, float> m_curves = {};
};