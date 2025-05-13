#include "AFGLTFLoader.h"

#include <glm/gtc/type_ptr.hpp>

#include "third_party/tiny_gltf.h"

bool AFGLTFLoader::Load(const std::string& filename, FAFMeshLoaded& loadedMesh, bool binary)
{
	tinygltf::Model model;
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	bool ret = binary ? 
		loader.LoadBinaryFromFile(&model, &err, &warn, filename) :
		loader.LoadASCIIFromFile(&model, &err, &warn, filename);

	if (!ret)
	{
		printf("Failed to parse glTF.\n");
		return false;
	}

	loadedMesh.subMeshesLoaded.clear();

	GLuint VAO = 0;
	std::vector<GLuint> VBO = {};
	GLuint EBO = 0;
	std::map<std::string, GLint> attributes = { {"POSITION", 0}, {"NORMAL", 1}, {"TEXCOORD_0", 2} };

	// Build bones tree.
	const int boneCount = model.nodes.size();
	const int rootBoneID = model.scenes.at(0).nodes.at(0);
	std::shared_ptr<AFBone> rootBone = AFBone::CreateRoot(rootBoneID);

	auto GetBoneData = [model](std::shared_ptr<AFBone> bone, const glm::mat4& boneMatrix) -> void
		{
			int boneID = bone->GetBoneID();
			const tinygltf::Node& node = model.nodes.at(boneID);
			bone->SetBoneName(node.name);

			if (!node.translation.empty()) 
			{
				bone->SetLocation(glm::make_vec3(node.translation.data()));
			}
			if (!node.rotation.empty()) 
			{
				bone->SetRotation(glm::make_quat(node.rotation.data()));
			}
			if (!node.scale.empty()) 
			{

				bone->SetScale(glm::make_vec3(node.scale.data()));
			}

			bone->CalculateLocalTRSMatrix();
			bone->CalculateNodeMatrix(boneMatrix);
		};

	std::function<void(std::shared_ptr<AFBone>)> GetBones;
	GetBones = [model, GetBoneData, &GetBones](std::shared_ptr<AFBone> bone) -> void
		{
			int boneID = bone->GetBoneID();
			std::vector<int> childBones = model.nodes.at(boneID).children;

			// ? - book quirks.
			// Remove the child node without skin/mesh metadata, confuses skeleton.
			auto removeIt = std::remove_if(childBones.begin(), childBones.end(),
				[&](int num) {return model.nodes.at(num).skin != -1; });
			childBones.erase(removeIt, childBones.end());

			bone->AddChildren(childBones);
			const glm::mat4& boneMatrix = bone->GetBoneMatrix();

			for(auto& childBone : bone->GetChildren())
			{
				GetBoneData(childBone, boneMatrix);
				GetBones(childBone);
			}
		};

	GetBoneData(rootBone, glm::mat4(1.0f));
	GetBones(rootBone);

	rootBone->PrintTree();

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

				// For now we just try to import positions, normals and uvs.
				if ((attribType != "POSITION") &&
					(attribType != "NORMAL") &&
					(attribType != "TEXCOORD_0"))
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
				default:
				{
					printf("glTF error, accessor %i uses unkown data type %i\n", accessorNum, dataType);
					break;
				}
				}

				// Create vertex buffer.
				glGenBuffers(1, &VBO[attributes[attribType]]);
				glBindBuffer(GL_ARRAY_BUFFER, VBO[attributes[attribType]]);

				// Configure VAO.
				glVertexAttribPointer(attributes[attribType], dataSize, dataType, GL_FALSE, 0, nullptr);
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
