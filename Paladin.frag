#version 330
// �o�̓J���[
layout(location = 0) out vec4 out_FragColor;

// ���[���h���W�n�̈ʒu
in vec4 v_WorldPosition;
// ���[���h���W�n�̖@���x�N�g��
in vec3 v_WorldNormal;
// ���[���h���W�n�̐ڃx�N�g��
in vec3 v_WorldTangent;
// ���[���h���W�n�̏]�@���x�N�g��
in vec3 v_WorldBinormal;
// �e�N�X�`�����W
in vec2 v_TexCoord;

// �x�[�X�J���[�e�N�X�`��
uniform sampler2D g_BaseMap;
// �@���}�b�v�e�N�X�`��
uniform sampler2D u_NormalMap;

void main(void) {
    // �@���}�b�v�J���[�̎擾
    vec4 normal = texture(u_NormalMap, v_TexCoord);
    // �e�N�X�`���J���[�̎擾
    vec4 baseColor = texture(g_BaseMap, v_TexCoord);

    // �ŏI�I�ȃJ���[���o��
    out_FragColor = vec4(baseColor.rgb, 1.0);
}