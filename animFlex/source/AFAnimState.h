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
	const AFPose& GetPose() const;
	void OnEventFromClip(const std::string& event);

	float GetCurveValue(const std::string& curveName) const;

	void SetEvaluationState(EAFAnimEvaluationState newEvaluationState);

	void CallFunctionByString(const std::string& functionName);

	// Graph functions.
	float GetStartRunDistanceMatchingTime() const;
	std::string GetStartRunAnim() const;
	std::string GetStartRunCurve_RootDistance() const;
	std::string GetStartRunCurve_RootYaw() const;
	float GetStartRunDistanceTraveled() const;
	float GetStartRunDifferenceToInput() const;
	float GetStartRunTimeSpent() const;
	float GetRootYaw() const;
	void SetRootYaw(float yaw);
	bool GetLeftFeetLocked() const;
	bool GetRightFeetLocked() const;
	float GetStopRunDistanceMatchingTime() const;
	std::string GetStopRunAnim() const;
	std::string GetStopRunCurve_RootSpeed() const;
	float GetStopRunDistanceRemaining() const;
	float GetRotateInPlacePlayTime() const;
	std::string GetRotateInPlaceAnim() const;

private:

	void EvaluateSingleAnim();
	void PreEvaluateGraph(float deltaTime);
	void EvaluateGraph(float deltaTime);

	// Graph functions.
	void OnStartRunEnter();
	void OnStartRunTick();
	void OnStopRunEnter();
	void OnStopRunTick();
	void OnRotateInPlaceEnter();
	void OnRotateInPlaceTick();

	// Graph variables.
	float m_startRunDistanceMatchingTime = 0.0f;
	std::string m_startRunAnim = "";
	std::string m_startRunCurve_rootDistance = "";
	std::shared_ptr<AFFloatCurve> m_startRunCurve_rootDistanceCrv = nullptr;
	float m_startRunDistanceMatchingTimeOffset = 0.0f;
	std::string m_startRunCurve_rootYaw = "";
	std::shared_ptr<AFFloatCurve> m_startRunCurve_rootYawCrv = nullptr;
	float m_startRunTimeSpent = 0.0f;
	float m_startRunDistanceTraveled = 0.0f;
	float m_startRunDifferenceToInput = 0.0f;
	float m_rootYaw = 0.0f;
	bool m_leftFeetLocked = false;
	bool m_rightFeetLocked = false;
	glm::vec3 m_cachedLocalMovementInput = glm::vec3(0.0f);
	float m_stopRunDistanceTraveled = 0.0f;
	std::string m_stopRunAnim = "";
	std::string m_stopRunCurve_rootDistance = "";
	std::string m_stopRunCurve_rootSpeed = "";
	std::shared_ptr<AFFloatCurve> m_stopRunCurve_rootDistanceCrv = nullptr;
	float m_stopRun_beginTime = 0.0f;
	float m_stopRun_distToZero = 0.0f;
	float m_stopRun_distToZeroCharMov = 0.0f;
	float m_stopRun_distanceMatchingTime = 0.0f;
	float m_stopRun_distanceRemaining = 0.0f;
	std::string m_rotateInPlace_anim = "";
	std::string m_rotateInPlace_rootYaw = "";
	std::shared_ptr<AFFloatCurve> m_rotateInPlace_curve_rootYaw = nullptr;
	float m_rotateInPlace_playTime = 0.0f;

	EAFAnimSourceState m_sourceState = EAFAnimSourceState::Graph;
	EAFAnimEvaluationState m_evaluationState = EAFAnimEvaluationState::Playing;
	AFSkeletalMeshComponent* m_ownerMesh = nullptr;
	AFPose m_pose = {};

	std::shared_ptr<AFAnimationClip> m_singleAnim = nullptr;
	std::shared_ptr<AFAnimGraph> m_graph = nullptr;

	std::unordered_map<std::string, float> m_curves = {};
};