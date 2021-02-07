#version 330
//�o�̓J���[
layout(location = 0) out vec4 out_FragColor;

//���[���h���W�n�̈ʒu
in vec4 v_WorldPosition;
//���[���h���W�n�̖@���x�N�g��
in vec3 v_WorldNormal;
//���[���h���W�n�̐ڃx�N�g��
in vec3 v_WorldTangent;
//���[���h���W�n�̏]�@���x�N�g��
in vec3 v_WorldBinormal;
//�e�N�X�`�����W
in vec2 v_TexCoord;

//�J�����̈ʒu(���[���h���W�n)
uniform vec3 u_CameraPosition;

//���C�g�̈ʒu
uniform vec3 u_LightPosition;
//���C�g�̊����p�����[�^
uniform vec4 u_LightAmbient;
//���C�g�̊g�U���ˌ��̃p�����[�^
uniform vec4 u_LightDiffuse;
//���C�g�̋��ʔ��ˌ��̃p�����[�^
uniform vec4 u_LightSpecular;

//�}�e���A���̊����˗�
uniform vec4 u_MaterialAmbient;
//�}�e���A���̊g�U���ˌ��̔��˗�
uniform vec4 u_MaterialDiffuse;
//�}�e���A���̋��ʔ��ˌ��̔��˗�
uniform vec4 u_MaterialSpecular;
//�}�e���A���̕��ˌ��J���[
uniform vec4 u_MaterialEmission;
//�}�e���A���̋��ʔ��ˌ��w��
uniform float u_MaterialShininess;

//�x�[�X�J���[�e�N�X�`��
uniform sampler2D u_BaseMap;
//�@���}�b�v�e�N�X�`��
uniform sampler2D u_NormalMap;

void main(void)
{
//�@���}�b�v�̖@���x�N�g�����擾
vec3 normal = texture(u_NormalMap,v_TexCoord).xyz*2.0-1.0;
//�@���}�b�v�p�̍��W�ϊ��s����쐬
mat3 TBN = mat3(normalize(v_WorldTangent),
				normalize(v_WorldBinormal),
				normalize(v_WorldNormal));

//�@���}�b�v�̖@���x�N�g�������[���h���W�n�ɕϊ�
vec3 N =normalize(TBN*normal);

//���C�g�̕����̃x�N�g�������߂�
vec3 L =normalize(u_LightPosition - v_WorldPosition.xyz);
//���_�����̃x�N�g�������߂�
vec3 V = normalize(u_CameraPosition-v_WorldPosition.xyz);
//2�����x�N�g��
vec3 H=normalize(L+V);

//�����̌v�Z
vec4 ambientColor=u_LightAmbient*u_MaterialAmbient;

//�g�U���ˌ��̌v�Z
float diffuse = max(dot(N,L),0.0f);
vec4 diffuseColor=u_LightDiffuse*u_MaterialDiffuse*diffuse;

//���ʔ��ˌ��̌v�Z
float specular=pow(max(dot(N,H),0.0f),u_MaterialShininess);
vec4 specularColor = u_LightSpecular*u_MaterialSpecular * specular;

//�e�N�X�`���J���[�̎擾
vec4 baseColor =texture(u_BaseMap,v_TexCoord);

//�����A�g�U���ˌ��A���ʔ��ˌ��A���ȕ��˂�����
vec4 color = (ambientColor + diffuseColor + u_MaterialEmission)*baseColor
				+specularColor;

//�ŏI�I�ȃJ���[���o��
out_FragColor =vec4(color.rgb,baseColor.a);

}