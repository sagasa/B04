#version 330

//�o�̓J���[
layout(location = 0)out vec4 out_FragColor;


//���[���h���W�n�̈ʒu
in vec4 v_WorldPosition;
//���[���h���W�n�̖@���x�N�g��
in vec3 v_WorldNormal;
//�e�N�X�`�����W
in vec2 v_TexCoord;


//�x�[�X�J���[�e�N�X�`��
uniform sampler2D g_BaseMap;


void main(void){

//�x�[�X�J���[
vec4 color = texture(g_BaseMap,v_TexCoord);

//�ŏI�I�ȃJ���[���o��
out_FragColor = color;

}