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

// This class acts as AnimInstance from Unreal Engine.
// It has lots of variables and functions used during graph evaluation.

class AFAnimState
{
public:

	void PreTick(float deltaTime);
	void Tick(float deltaTime);

	void SetOwnerMesh(AFSkeletalMeshComponent* newOwner);

	void SetAnimation(std::shared_ptr<AFAnimationClip> animation);
	void SetGraph(std::shared_ptr<AFAnimGraph> animGraph);
	std::shared_ptr<AFAnimGraph> GetGraph() const;

	float GetCurveValue(const std::string& curveName) const;

	void SetEvaluationState(EAFAnimEvaluationState newEvaluationState);

	void CallFunctionByString(const std::string& functionName);

	// Graph functions.
	std::string GetStartRunAnim() const;
	std::string GetStartRunCurve_RootDistance() const;
	std::string GetStartRunCurve_RootYaw() const;
	float GetStartRunDistanceTraveled() const;
	float GetRootYaw() const;
	void SetRootYaw(float yaw);

private:

	void EvaluateSingleAnim();
	void PreEvaluateGraph(float deltaTime);
	void EvaluateGraph(float deltaTime);

	// Graph functions.
	void OnStartRunEnter();

	// Graph variables.
	std::string m_startRunAnim = "";
	std::string m_startRunCurve_rootDistance = "";
	std::string m_startRunCurve_rootYaw = "";
	float m_startRunDistanceTraveled = 0.0f;
	float m_rootYaw = 0.0f;

	EAFAnimSourceState m_sourceState = EAFAnimSourceState::Graph;
	EAFAnimEvaluationState m_evaluationState = EAFAnimEvaluationState::Playing;
	AFSkeletalMeshComponent* m_ownerMesh = nullptr;

	std::shared_ptr<AFAnimationClip> m_singleAnim = nullptr;
	std::shared_ptr<AFAnimGraph> m_graph = nullptr;

	std::unordered_map<std::string, float> m_curves = {};
};