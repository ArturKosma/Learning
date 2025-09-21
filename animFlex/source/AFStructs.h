#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>

#define AFENUM() 

struct FAFCameraProperties
{
	int fieldOfView = 60;
	float near = 20.0f;
	float far = 20000.0f;
};

struct FAFSceneData
{
	std::vector<std::shared_ptr<class AFActor>> sceneActors = {};
	std::vector<std::shared_ptr<class AFUI>> uis = {};
	unsigned long long vertexCount = 0;
	std::shared_ptr<class AFCameraComponent> activeCamera = nullptr;
	std::shared_ptr<class AFPlayerPawn> playerPawn = nullptr;
};

struct FAFVertex
{
	FAFVertex() = default;
	FAFVertex(const glm::vec3 newPosition)
		: position(newPosition), normal(glm::vec3(0.0f)), uv(glm::vec2(0.0f)), faceID(0), uvCenter(glm::vec2(0.0f))
	{
		
	}
	FAFVertex(const glm::vec3& newPosition, const glm::vec3& newNormal, const glm::vec2& newUV, glm::uint8_t newFaceID = 0, glm::vec2 newUVCenter = glm::vec2())
		: position(newPosition), normal(newNormal), uv(newUV), faceID(newFaceID), uvCenter(newUVCenter)
	{

	}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::uint8_t faceID;
	glm::vec2 uvCenter;
	glm::uvec4 jointNum;
	glm::vec4 jointWeight;
	glm::vec3 color;
};

enum class EAFTargetPath : uint8_t
{
	Rotation,
	Translation,
	Scale
};

struct FAFSubMeshLoaded
{
	GLuint vao;
	GLuint drawMode;
	GLsizei drawCount;
	GLenum drawType;
};

struct FAFMeshLoaded
{
	std::vector<FAFSubMeshLoaded> subMeshesLoaded = {};
	std::shared_ptr<class AFJoint> rootJoint = nullptr;
	std::vector<int> nodeToJoint = {};
	std::vector<std::shared_ptr<AFJoint>> joints = {};
	std::vector<glm::mat4> inverseBindMatrices = {};
	std::vector<glm::mat4> jointMatrices = {};
	std::vector<glm::mat4> jointDualQuats = {};
};

enum class EAFDrawType : uint8_t
{
	Normal,
	IDPicker
};

struct FAFAppData
{
	struct GLFWwindow* window = nullptr;
	int width = 0;
	int height = 0;
	EAFDrawType drawType = EAFDrawType::Normal;
};

struct FAFDrawOverride
{
	EAFDrawType drawType;
	std::shared_ptr<class AFShader> shader;
};

struct FAFPickID
{
	uint32_t objectId = 0;
	uint8_t elementId = 0;
};

enum class EAFCameraMovementMode : uint8_t
{
	Normal,
	Blending
};

enum class EAFInterpolationType : uint8_t
{
	Step,
	Linear,
	CubicSpline,
	CubicHermite
};

struct FAFGlyph
{
	unsigned int textureId = 0;
	glm::ivec2 size = glm::ivec2(0);
	glm::ivec2 bearing = glm::ivec2(0);
	unsigned int advance = 0;
};

template <typename T>
struct FAFFetchContext
{
	std::string fullPath;
	std::string assetName;
	std::shared_ptr<T> ret;
	class AFContent* content;
	std::function<void(std::shared_ptr<T>)> onComplete;
};

struct FAFStateConnection
{
	std::string from;
	std::string to;
	std::string id;
	std::string cond;
};

enum class EAFControlMode : uint8_t
{
	Editor,
	ActionRPG
};

enum class EAFEditorControlMode: uint8_t
{
	Normal,
	FreeView
};

enum class EAFCameraManagerState: uint8_t
{
	Editor,
	Blending,
	ActionRPG
};

enum class EAFBlendDirection: uint8_t
{
	Forward,
	Backward
};

struct FAFBlendStackEvalParams
{
	float time = 0.0f;
	bool rootLock = true;
};

struct IAFBlendStack_Node
{
	virtual ~IAFBlendStack_Node() = default;
	virtual void Evaluate(float deltaTime, class AFPose& pose, const FAFBlendStackEvalParams& params = {}) = 0;
};

struct FAFBlendStack_Evaluator final : public IAFBlendStack_Node
{
	void Evaluate(float deltaTime, AFPose& pose, const FAFBlendStackEvalParams& params = {}) override;

	std::shared_ptr<class AFGraphNode_State> state = nullptr;
	std::shared_ptr<class AFAnimationClip> clip = nullptr;
};

struct FAFBlendStack_Blender final : public IAFBlendStack_Node
{
	void Evaluate(float deltaTime, AFPose& pose, const FAFBlendStackEvalParams& params = {}) override;
	void ProgressBlendTime(float deltaTime);
	bool HasFinished() const;
	float GetBlendTime() const;

	float t = 0.0f;
	float duration = 0.25f;
	std::shared_ptr<IAFBlendStack_Node> from = nullptr;
	std::shared_ptr<FAFBlendStack_Evaluator> to = nullptr;
	EAFBlendDirection direction = EAFBlendDirection::Forward;
};

struct FAFStateSampling
{
	std::string nodeId = "";
	std::string contextId = "";
	float sampleTime = 0.0f;
	float maxTime = 0.0f;
};

enum class EAFEvaluationMode : uint8_t
{
	PreEvaluate,
	Evaluate
};

AFENUM()
enum class EAFColor
{
	Red,
	Green,
	Blue
};

enum class EAFDebugShape : uint8_t
{
	Box
};

AFENUM()
enum class EAFBoneProperty
{
	Translation,
	Rotation
};

AFENUM()
enum class EAFBoneSpace
{
	Global,
	Local,
	World
};

enum class EAFJointType : uint8_t
{
	Hinge,
	Ball
};

struct FAFHingeProperties
{
	glm::vec3 hingeAxis = glm::vec3(1.0f, 0.0f, 0.0f);
	float minAngle = 0.0f;
	float maxAngle = 180.0f;
};

struct FAFBallProperties
{
	
};

struct FAFIKBoneProperties
{
	std::string boneName = "";
	EAFJointType boneType = EAFJointType::Hinge;

	FAFHingeProperties hingeProperties = {};
	FAFBallProperties ballProperties = {};
};

AFENUM()
enum class EAFSyncGroupMode
{
	Driver,
	Listener
};

struct FAFSyncGroupProperties
{
	std::string name = "";
	EAFSyncGroupMode mode = EAFSyncGroupMode::Driver;
};

enum class EAFEventTiming
{
	Before,
	After
};
