#include "AFGLTFLoader.h"

#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <filesystem>

#include "third_party/tiny_gltf.h"

bool AFGLTFLoader::Load(const std::string& filename, FAFMeshLoaded& loadedMesh)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	bool ret = false;

	ret = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
	if(!ret)
	{
		ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
	}

	if (!ret)
	{
		printf("Failed to parse glTF.\n");
		return false;
	}

	loadedMesh.subMeshesLoaded.clear();

	GLuint VAO = 0;
	std::vector<GLuint> VBO = {};
	GLuint EBO = 0;
	std::map<std::string, GLint> attributes = { {"POSITION", 0}, {"NORMAL", 1}, {"TEXCOORD_0", 2}, {"JOINTS_0", 3}, {"WEIGHTS_0", 4} };

	// Create the necessary containers.
	std::vector<int> nodeToJoint = {}; // Maps node to a joint in a simple way for quick access of different vectors.
	std::vector<std::shared_ptr<AFNode>> joints = {}; // Main map containing whole skeleton.
	std::vector<glm::mat4> inverseBindMatrices = {};
	std::vector<glm::mat4> jointMatrices = {};
	std::vector<glm::mat4> jointDualQuats = {};

	const tinygltf::Skin& skin = model.skins.at(0); // We assume single skin files.

	nodeToJoint.resize(model.nodes.size(), -1);
	joints.resize(skin.joints.size());
	inverseBindMatrices.resize(skin.joints.size());
	jointMatrices.resize(skin.joints.size());
	jointDualQuats.resize(skin.joints.size());

	// Fetch the inverse bind matrices from buffer.	
	const int invBindMatAccessor = skin.inverseBindMatrices;

	const tinygltf::Accessor& accessor = model.accessors.at(invBindMatAccessor);
	const tinygltf::BufferView& bufferView = model.bufferViews.at(accessor.bufferView);
	const tinygltf::Buffer& buffer = model.buffers.at(bufferView.buffer);

	std::memcpy(inverseBindMatrices.data(), &buffer.data.at(0) + bufferView.byteOffset, bufferView.byteLength);

	// Create a mapping from nodes to joints.
	for(size_t i = 0; i < skin.joints.size(); ++i)
	{
		const int targetNode = skin.joints.at(i);
		nodeToJoint.at(targetNode) = static_cast<int>(i);
	}

	auto GetNodeData = [model, &jointMatrices, &inverseBindMatrices, &nodeToJoint, &jointDualQuats, &joints](std::shared_ptr<AFNode> nodeToCalculate, const glm::mat4& nodeMatrix) -> void
		{
			int nodeID = nodeToCalculate->GetNodeID();
			const tinygltf::Node& node = model.nodes.at(nodeID);
			nodeToCalculate->SetNodeName(node.name);

			glm::vec3 translation = glm::vec3(0.0f), scale = glm::vec3(1.0f), skew;
			glm::vec4 perspective;
			glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
			glm::dualquat dq;

			if (!node.matrix.empty())
			{
				const glm::mat4& mat = glm::make_mat4(node.matrix.data());

				if (glm::decompose(mat, scale, rotation, translation, skew, perspective))
				{
					
				}
			}
			else
			{
				if (!node.translation.empty())
				{
					translation = glm::make_vec3(node.translation.data());
				}
				if (!node.rotation.empty())
				{
					rotation = glm::make_quat(node.rotation.data());
				}
				if (!node.scale.empty())
				{
					scale = glm::make_vec3(node.scale.data());
				}
			}

			nodeToCalculate->SetLocation(translation);
			nodeToCalculate->SetRotation(rotation);
			nodeToCalculate->SetScale(scale);

			nodeToCalculate->CalculateLocalTRSMatrix();
			nodeToCalculate->CalculateNodeMatrix(nodeMatrix);

			if (glm::decompose(nodeToCalculate->GetNodeMatrix() * inverseBindMatrices.at(nodeToJoint.at(nodeID)), 
				scale, rotation, translation, skew, perspective))
			{
				// Fill dual quaternions. @see C++ Game Animation Programming by Dunsky & Szauer.
				dq[0] = rotation;
				dq[1] = glm::quat(0.0f, translation.x, translation.y, translation.z) * rotation * 0.5f;
				glm::mat2x4 dualQuatJoint = glm::mat2x4_cast(dq);
				glm::mat4 paddedMat(0.0f); // Web-GL doesn't accept 2x4.
				paddedMat[0] = dualQuatJoint[0];
				paddedMat[1] = dualQuatJoint[1];
				jointDualQuats.at(nodeToJoint.at(nodeID)) = paddedMat;
			}

			joints.at(nodeToJoint.at(nodeID)) = nodeToCalculate;
			jointMatrices.at(nodeToJoint.at(nodeID)) = nodeToCalculate->GetNodeMatrix() * inverseBindMatrices.at(nodeToJoint.at(nodeID));
		};

	std::function<void(std::shared_ptr<AFNode>)> GetNodes;
	GetNodes = [model, GetNodeData, &GetNodes, &joints](std::shared_ptr<AFNode> nodeToFill) -> void
		{
			int nodeID = nodeToFill->GetNodeID();

			std::vector<int> childNodes = model.nodes.at(nodeID).children;

			// Remove the child nodes with the skin metadata, it's not part of the skeleton - it could be a mesh, or a mistake in export.
			auto removeIt = std::remove_if(childNodes.begin(), childNodes.end(),
				[&](int num) {return model.nodes.at(num).skin != -1; });
			childNodes.erase(removeIt, childNodes.end());

			nodeToFill->AddChildren(childNodes);
			const glm::mat4& nodeMatrix = nodeToFill->GetNodeMatrix();

			for(auto& childNode : nodeToFill->GetChildren())
			{
				GetNodeData(childNode, nodeMatrix);
				GetNodes(childNode);
			}
		};

	// Find the root bone node ID.
	const size_t rootNodeID = skin.joints.at(0);
	std::shared_ptr<AFNode> rootNode = AFNode::CreateRoot(static_cast<int>(rootNodeID));

	// Fill info for the root node.
	GetNodeData(rootNode, glm::mat4(1.0f));

	// Fill info for the rest of nodes recurrently, essentially creating the default pose and whole skeleton tree.
	GetNodes(rootNode);

	//rootNode->PrintTree();

	// Fill bones info for the loaded mesh temp object.
	loadedMesh.rootJoint = rootNode;
	loadedMesh.nodeToJoint = nodeToJoint;
	loadedMesh.joints = joints;
	loadedMesh.inverseBindMatrices = inverseBindMatrices;
	loadedMesh.jointMatrices = jointMatrices;
	loadedMesh.jointDualQuats = jointDualQuats;

	// Create VAO for each sub-mesh.
	for(int meshIdx = 0; meshIdx < model.meshes.size(); ++meshIdx)
	{
		for (int primIdx = 0; primIdx < model.meshes.at(meshIdx).primitives.size(); ++primIdx)
		{
			const tinygltf::Primitive& primitives = model.meshes.at(meshIdx).primitives.at(primIdx);
			VBO.resize(primitives.attributes.size());

			// Create & bind VAO.
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			for (const auto& attrib : primitives.attributes)
			{
				const std::string attribType = attrib.first;
				const int accessorNum = attrib.second;

				const tinygltf::Accessor& accessor = model.accessors[accessorNum];
				const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
				const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

				if ((attribType != "POSITION") &&
					(attribType != "NORMAL") &&
					(attribType != "TEXCOORD_0") &&
					(attribType != "JOINTS_0") &&
					(attribType != "WEIGHTS_0"))
				{
					continue;
				}

				int dataSize = 1;
				switch (accessor.type)
				{
				case TINYGLTF_TYPE_SCALAR:
				{
					dataSize = 1;
					break;
				}
				case TINYGLTF_TYPE_VEC2:
				{
					dataSize = 2;
					break;
				}
				case TINYGLTF_TYPE_VEC3:
				{
					dataSize = 3;
					break;
				}
				case TINYGLTF_TYPE_VEC4:
				{
					dataSize = 4;
					break;
				}
				default:
				{
					printf("glTF error, accessor %i uses data size %i\n", accessorNum, dataSize);
					break;
				}
				}

				GLuint dataType = GL_FLOAT;
				switch (accessor.componentType)
				{
				case TINYGLTF_COMPONENT_TYPE_FLOAT:
				{
					dataType = GL_FLOAT;
					break;
				}
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
				{
					dataType = GL_UNSIGNED_SHORT;
					break;
				}
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
				{
					dataType = GL_UNSIGNED_BYTE;
					break;
				}
				default:
				{
					printf("glTF error, accessor %i uses unkown data type %i\n", accessorNum, accessor.componentType);
					break;
				}
				}

				// Create vertex buffer.
				glGenBuffers(1, &VBO[attributes[attribType]]);
				glBindBuffer(GL_ARRAY_BUFFER, VBO[attributes[attribType]]);

				// Configure VAO.
				if(dataType == GL_UNSIGNED_SHORT || dataType == GL_UNSIGNED_BYTE) // #hack. Allow nicer check if we talk about integers.
				{
					glVertexAttribIPointer(attributes[attribType], dataSize, dataType, 0, nullptr);
				}
				else
				{
					glVertexAttribPointer(attributes[attribType], dataSize, dataType, GL_FALSE, 0, nullptr);
				}
				glEnableVertexAttribArray(attributes[attribType]);
			}

			// Upload VBOs.
			for (const auto& attrib : primitives.attributes)
			{
				const std::string attribType = attrib.first;
				if (attributes.find(attribType) == attributes.end())
				{
					continue;
				}

				const int accessorNum = attrib.second;
				const tinygltf::Accessor& accessor = model.accessors[accessorNum];
				const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
				const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

				glBindBuffer(GL_ARRAY_BUFFER, VBO[attributes[attribType]]);
				glBufferData(GL_ARRAY_BUFFER, bufferView.byteLength, &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			const tinygltf::Accessor& indexAccessor = model.accessors[primitives.indices];
			const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];
			const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];

			// Create & bind EBO.
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferView.byteLength, &indexBuffer.data.at(0) + indexBufferView.byteOffset, GL_STATIC_DRAW);

			// Unbind everything.
			glBindBuffer(GL_ARRAY_BUFFER, 0); // Probably unnecessary as I unbind VBO after glBufferData upload.
			glBindVertexArray(0);

			FAFSubMeshLoaded subMesh;
			subMesh.vao = VAO;
			subMesh.drawMode = primitives.mode;
			subMesh.drawCount = static_cast<GLsizei>(indexAccessor.count);
			subMesh.drawType = indexAccessor.componentType;

			loadedMesh.subMeshesLoaded.push_back(subMesh);
		}
	}	

	return true;
}

bool AFGLTFLoader::LoadAnim(const std::string& filename, AFAnimationClip* loadedClip)
{
	std::shared_ptr<tinygltf::Model> model = std::make_shared<tinygltf::Model>();
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	bool ret = loader.LoadBinaryFromFile(&*model, &err, &warn, filename);
	if(!ret)
	{
		ret = loader.LoadASCIIFromFile(&*model, &err, &warn, filename);
	}

	if (!ret)
	{
		printf("Failed to parse glTF anim.\n");
		return false;
	}

	if (model->animations.empty())
	{
		printf("No animations in file.\n");
		return false;
	}

	if(!loadedClip)
	{
		return false;
	}

	// Save just the filename, without extension nor directory.
	std::filesystem::path p(filename);
	loadedClip->SetClipName(p.filename().stem().string());

	// We assume to have only 1 animation per file.
	for (const auto& anim : model->animations)
	{
		for (const auto& channel : anim.channels)
		{
			loadedClip->AddChannel(model, anim, channel);
		}
	}

	return true;
}
