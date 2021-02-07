#version 330

layout(location = 0)in vec4 in_Position;

layout(location = 1)in vec4 in_BoneWeight;

layout(location = 2)in vec3 in_Normal;

layout(location = 7)in vec4 in_BoneIndices;

layout(location = 8)in vec2 in_TexCoord;

layout(location = 14)in vec3 in_Tangent;

layout(location = 15) in vec3 in_Binormal;

out vec4 v_WorldPosition;

out vec3 v_WorldNormal;

out vec3 v_WorldTangent;

out vec3 v_WorldBinormal;

out vec2 v_TexCoord;

uniform mat4 u_WorldMatrix;
uniform mat4 u_WorldViewProjectionMatrix;

uniform mat4 gs_BoneMatrices[128];

void main(void){
	mat4 matLocal=gs_BoneMatrices[int(in_BoneIndices.x)]*in_BoneWeight.x
					+gs_BoneMatrices[int(in_BoneIndices.y)]*in_BoneWeight.y
					+gs_BoneMatrices[int(in_BoneIndices.z)]*in_BoneWeight.z
					+gs_BoneMatrices[int(in_BoneIndices.w)]*in_BoneWeight.w;
	vec4 localPosition=matLocal*in_Position;
	vec3 localNormal=mat3(matLocal)*in_Normal;
	vec3 localTangent=mat3(matLocal)*in_Tangent;
	vec3 localBinormal=mat3(matLocal)*in_Binormal;

	v_WorldPosition=u_WorldMatrix*localPosition;

	v_WorldNormal=mat3(u_WorldMatrix)*localNormal;

	v_WorldTangent=mat3(u_WorldMatrix)*localTangent;

	v_WorldBinormal=mat3(u_WorldMatrix)*localBinormal;

	v_TexCoord=in_TexCoord;

	gl_Position=u_WorldViewProjectionMatrix*localPosition;
}