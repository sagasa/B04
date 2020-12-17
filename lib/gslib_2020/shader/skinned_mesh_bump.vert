#version 330
layout(location = 0) in vec4 in_Position;
layout(location = 1) in vec4 in_BoneWeight;
layout(location = 2) in vec3 in_Normal;
layout(location = 7) in vec4 in_BoneIndices;
layout(location = 8) in vec2 in_TexCoord;
layout(location = 14) in vec3 in_Tangent;
layout(location = 15) in vec3 in_Binormal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_View;
out vec3 v_Light;

uniform vec3 gs_LightPosition;

uniform mat4 gs_ModelViewMatrix;
uniform mat4 gs_ModelViewProjectionMatrix;
uniform mat4 gs_NormalMatrix;
uniform mat4 gs_BoneMatrices[128];

void main(void) {
	mat4 matLocal = gs_BoneMatrices[int(in_BoneIndices.x)] * in_BoneWeight.x
				  + gs_BoneMatrices[int(in_BoneIndices.y)] * in_BoneWeight.y
				  + gs_BoneMatrices[int(in_BoneIndices.z)] * in_BoneWeight.z
				  + gs_BoneMatrices[int(in_BoneIndices.w)] * in_BoneWeight.w;
	vec4 localPosition = matLocal * in_Position;
	vec3 localNormal   = mat3(matLocal) * in_Normal;
	vec3 localTangent  = mat3(matLocal) * in_Tangent;
	vec3 localBinormal = mat3(matLocal) * in_Binormal;
	vec4 viewPosition  = gs_ModelViewMatrix * localPosition;
	vec3 viewNormal    = mat3(gs_NormalMatrix) * localNormal;
	vec3 viewTangent   = mat3(gs_NormalMatrix) * localTangent;
	vec3 viewBinormal  = mat3(gs_NormalMatrix) * localBinormal;
	mat3 matTBN = mat3(normalize(viewTangent), normalize(viewBinormal), normalize(viewNormal));

	v_TexCoord = in_TexCoord;
	v_View = vec3(-viewPosition) * matTBN;
	v_Light = (gs_LightPosition - vec3(viewPosition)) * matTBN;
	gl_Position = gs_ModelViewProjectionMatrix * localPosition;
}
