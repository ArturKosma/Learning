#version 300 es
precision highp float;

layout (std140) uniform ViewProjection
{
	mat4 view;
	mat4 projection;
};

layout (std140) uniform Model
{
	mat4 modelTransform;
};

layout (std140) uniform JointMatrices
{
	mat4 jointMat[200]; // Hardcoded number of joints. #todo variable this.
};

layout (std140) uniform JointDualQuats
{
	mat4 jointDQs[200]; // Hardcoded number of joints. #todo variable this.
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
layout (location = 3) in uvec4 aJointNum;
layout (location = 4) in vec4 aJointWeight;

out vec4 Pos;
out vec3 Normal;
out vec2 UV;

mat2x4 GetJointTransform(uvec4 joints, vec4 weights)
{
	// Read dual quats from the buffer.
	mat2x4 dq0 = mat2x4(jointDQs[joints.x][0], jointDQs[joints.x][1]);
	mat2x4 dq1 = mat2x4(jointDQs[joints.y][0], jointDQs[joints.y][1]);
	mat2x4 dq2 = mat2x4(jointDQs[joints.z][0], jointDQs[joints.z][1]);
	mat2x4 dq3 = mat2x4(jointDQs[joints.w][0], jointDQs[joints.w][1]);

	// Shortest rotation.
	weights.y *= sign(dot(dq0[0], dq1[0]));
	weights.z *= sign(dot(dq0[0], dq2[0]));
	weights.w *= sign(dot(dq0[0], dq3[0]));

	// Blend.
	mat2x4 result =
      weights.x * dq0 +
      weights.y * dq1 +
      weights.z * dq2 +
      weights.w * dq3;

	// Normalize the dual quaternion.
	float norm = length(result[0]);
	return result / norm;
}

mat4 DualQuatSkinMat()
{
	mat2x4 bone = GetJointTransform(aJointNum, aJointWeight);

	vec4 r = bone[0];
	vec4 t = bone[1];

	return mat4
	(
		1.0f - (2.0f * r.y * r.y) - (2.0f * r.z * r.z),
            (2.0f * r.x * r.y) + (2.0f * r.w * r.z),
            (2.0f * r.x * r.z) - (2.0f * r.w * r.y),
		0.0f,

            (2.0f * r.x * r.y) - (2.0f * r.w * r.z),
		1.0f - (2.0f * r.x * r.x) - (2.0f * r.z * r.z),
            (2.0f * r.y * r.z) + (2.0f * r.w * r.x),
		0.0f,

            (2.0f * r.x * r.z) + (2.0f * r.w * r.y),
            (2.0f * r.y * r.z) - (2.0f * r.w * r.x),
		1.0f - (2.0f * r.x * r.x) - (2.0f * r.y * r.y),
		0.0f,

		2.0f * (-t.w * r.x + t.x * r.w - t.y * r.z + t.z * r.y),
		2.0f * (-t.w * r.y + t.x * r.z + t.y * r.w - t.z * r.x),
		2.0f * (-t.w * r.z - t.x * r.y + t.y * r.x + t.z * r.w),
		1.0f 
	);
}

void main()
{
	mat4 skinMat = 
	aJointWeight.x * jointMat[aJointNum.x] +
	aJointWeight.y * jointMat[aJointNum.y] +
	aJointWeight.z * jointMat[aJointNum.z] +
	aJointWeight.w * jointMat[aJointNum.w];

	mat4 skinMatDq = DualQuatSkinMat();

	mat3 normalMatrix = mat3(transpose(inverse(mat3(skinMatDq))));
	vec3 worldNormal = normalize(normalMatrix * aNormal);

	vec4 worldPos = modelTransform * skinMatDq * vec4(aPos, 1.0f);

	Pos = worldPos;
	Normal = worldNormal;
	UV = aUV;

	gl_Position = projection * view * worldPos;
}