#version 330
// ���_���W�i���[�J�����W�n�j
layout(location = 0) in vec4 in_Position;
// �@���x�N�g���i���[�J�����W�n�j
layout(location = 2) in vec3 in_Normal;
// �e�N�X�`�����W
layout(location = 8) in vec2 in_TexCoord;
// �ڃx�N�g��
layout(location = 14) in vec3 in_Tangent;
// �]�@���x�N�g��
layout(location = 15) in vec3 in_Binormal;

// ���[���h���W�n�̈ʒu
out vec4 v_WorldPosition;
// ���[���h���W�n�̖@���x�N�g��
out vec3 v_WorldNormal;
// ���[���h���W�n�̐ڃx�N�g��
out vec3 v_WorldTangent;
// ���[���h���W�n�̏]�@���x�N�g��
out vec3 v_WorldBinormal;
// �e�N�X�`�����W
out vec2 v_TexCoord;

// ���[���h�ϊ��s��
uniform mat4 u_WorldMatrix;
// ���[���h�E�r���[�E�v���W�F�N�V�����ϊ��s��
uniform mat4 u_WorldViewProjectionMatrix;

void main(void) {
    // ���_���W�����[���h���W�n�ɕϊ�
    v_WorldPosition = u_WorldMatrix * in_Position;
    // �@���x�N�g�������[���h���W�n�ɕϊ�
    v_WorldNormal = mat3(u_WorldMatrix) * in_Normal;
    // �ڃx�N�g�������[���h���W�n�ɕϊ�
    v_WorldTangent = mat3(u_WorldMatrix) * in_Tangent;
    // �]�@���x�N�g�������[���h���W�n�ɕϊ�
    v_WorldBinormal = mat3(u_WorldMatrix) * in_Binormal;
    // �e�N�X�`�����W�̏o��
    v_TexCoord = in_TexCoord;
    // ���[���h�E�r���[�E�v���W�F�N�V�����ϊ�
    gl_Position = u_WorldViewProjectionMatrix * in_Position;
}