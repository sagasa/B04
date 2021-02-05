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

// �J�����̈ʒu�i���[���h���W�n�j
uniform vec3 u_CameraPosition;

// ���C�g�̈ʒu�i���[���h���W�n�j
uniform vec3 u_LightPosition;
// ���C�g�̊����p�����[�^ 
uniform vec4 u_LightAmbient;
// ���C�g�̊g�U���ˌ��̃p�����[�^
uniform vec4 u_LightDiffuse;
// ���C�g�̋��ʔ��ˌ��̃p�����[�^
uniform vec4 u_LightSpecular;

// �}�e���A���̊������˗�
uniform vec4 u_MaterialAmbient;
// �}�e���A���̊g�U���ˌ��̔��˗�
uniform vec4 u_MaterialDiffuse;
// �}�e���A���̋��ʔ��ˌ��̔��˗�
uniform vec4 u_MaterialSpecular;
// �}�e���A���̕��ˌ��J���[
uniform vec4 u_MaterialEmission;
// �}�e���A���̋��ʔ��ˌ��w��
uniform float u_MaterialShininess;

// �x�[�X�J���[�}�b�v�e�N�X�`��
uniform sampler2D u_BaseMap;
// �@���}�b�v�e�N�X�`��
uniform sampler2D u_NormalMap; 
// �X�y�L�����}�b�v�e�N�X�`��
uniform sampler2D u_SpecularMap;
// �A���r�G���g�I�N���[�W�����}�b�v
uniform sampler2D u_AmbientOcclusionMap;
// ���ˌ��}�b�v
uniform sampler2D u_EmissionMap;

void main(void) {
    // �@���}�b�v�̖@���x�N�g�����擾
    vec3 normal = texture(u_NormalMap, v_TexCoord).xyz * 2.0 - 1.0;
    // �@���}�b�v�p�̍��W�ϊ��s����쐬
    mat3 TBN = mat3(normalize(v_WorldTangent), 
                    normalize(v_WorldBinormal),
                    normalize(v_WorldNormal));
    // �@���}�b�v�̖@���x�N�g�������[���h���W�n�ɕϊ�
    vec3 N = normalize(TBN * normal);
    // ���C�g�����̃x�N�g�������߂�
    vec3 L = normalize(u_LightPosition - v_WorldPosition.xyz);
    // ���_�����̃x�N�g�������߂�i���[���h���W�n)
    vec3 V = normalize(u_CameraPosition - v_WorldPosition.xyz);
    // 2�����x�N�g�������߂�
    vec3 H = normalize(L + V);

    // �x�[�X�J���[�}�b�v�̃J���[���擾
    vec4 baseMapColor = texture(u_BaseMap, v_TexCoord);

    // �ŏI�I�Ȋ��������߂�
    vec4 ambientColor = u_LightAmbient * u_MaterialAmbient * baseMapColor;
    
    // �g�U���ˌ��̌v�Z
    float diffuse = max(dot(N, L), 0.0);
    // �ŏI�I�Ȋg�U���ˌ������߂�
    vec4 diffuseColor = u_LightDiffuse * u_MaterialDiffuse * baseMapColor * diffuse;
    
    // ���ʔ��ˌ��̌v�Z
    float specular = pow(max(dot(N, H), 0.0), u_MaterialShininess);
    // �X�y�L�����}�b�v�̃J���[���擾
    vec4 specularMapColor = texture(u_SpecularMap, v_TexCoord);
    // �ŏI�I�ȋ��ʔ��ˌ������߂�
    vec4 specularColor = u_LightSpecular * u_MaterialSpecular * specularMapColor * specular;

    // ���ˌ��}�b�v�̃J���[���擾
    vec4 emissionMapColor = texture(u_EmissionMap, v_TexCoord);
    // �ŏI�I�ȕ��ˌ������߂�
    vec4 emissionColor = u_MaterialEmission * emissionMapColor;

    // �����E�g�U���ˌ��E���ʔ��ˌ��E���ˌ�������
    vec4 finalColor = ambientColor + diffuseColor + specularColor + emissionColor;


    // �ŏI�I�ȃJ���[���o��
    out_FragColor = vec4(finalColor.rgb, baseMapColor.a);
}