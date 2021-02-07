#version 330
// ���_���W�i���[�J�����W�n�j
layout(location = 0) in vec4 in_Position;
// �{�[���̃E�F�C�g
layout(location = 1)  in vec4 in_BoneWeight;
// �@���x�N�g���i���[�J�����W�n�j
layout(location = 2) in vec3 in_Normal;
// �{�[���̔ԍ�
layout(location = 7)  in vec4 in_BoneIndices;  
// �e�N�X�`�����W
layout(location = 8) in vec2 in_TexCoord;

// ���[���h���W�n�̈ʒu
out vec4 v_WorldPosition;
// ���[���h���W�n�̖@���x�N�g��
out vec3 v_WorldNormal;
// �e�N�X�`�����W
out vec2 v_TexCoord;

// ���[���h�ϊ��s��
uniform mat4 u_WorldMatrix;
// ���[���h�E�r���[�E�v���W�F�N�V�����ϊ��s��
uniform mat4 u_WorldViewProjectionMatrix;

// �{�[���̕ϊ��s��
uniform mat4 gs_BoneMatrices[128];              

void main(void) {
    // �X�L�j���O�p�̕ϊ��s������߂�
    mat4 matLocal = gs_BoneMatrices[int(in_BoneIndices.x)] * in_BoneWeight.x
                  + gs_BoneMatrices[int(in_BoneIndices.y)] * in_BoneWeight.y
                  + gs_BoneMatrices[int(in_BoneIndices.z)] * in_BoneWeight.z
                  + gs_BoneMatrices[int(in_BoneIndices.w)] * in_BoneWeight.w;
    // �X�L�j���O��̃��[�J�����W�n�ɕϊ�
    vec4 localPosition = matLocal * in_Position;
    // �@���x�N�g���̃X�L�j���O�p�̍��W�ϊ�
    vec3 localNormal   = mat3(matLocal) * in_Normal;

    // ���_���W�����[���h���W�n�ɕϊ�
    v_WorldPosition = u_WorldMatrix * localPosition;
    // �@���x�N�g�������[���h���W�n�ɕϊ�
    v_WorldNormal = mat3(u_WorldMatrix) * localNormal;
    // �e�N�X�`�����W�̏o��
    v_TexCoord = in_TexCoord;
    // ���[���h�E�r���[�E�v���W�F�N�V�����ϊ�
    gl_Position = u_WorldViewProjectionMatrix * in_Position;
}