#include "Fade.h"
#include"Assets.h"



//�f�X�g���N�^
Fade::~Fade() {
	//����
	gsDeleteTexture(Texture_Fade);
}

//�J�n(�t�F�[�h�C���Ȃ�true,�t�F�[�h�A�E�g�Ȃ�false)
void Fade::start(bool fade_in,float fade_time) {
	//�t�F�[�h�C���Ȃ�true,�t�F�[�h�A�E�g�Ȃ�false
	fade_in_ = fade_in;
	//fa_in_�̒l�ŏ����l��ݒ�
	color_alpha_ = (fade_in_) ? 1.0f : 0.0f;
	//�I���t���O��������
	end_ = false;
	//���l�ɓ����l
	alpha_value_ = 0.016f / fade_time;
}

//�X�V
void Fade::update(float delta_time) {
	//���l�̍X�V
	color_alpha_ += (fade_in_) ? -alpha_value_ * delta_time : alpha_value_ * delta_time;
	//�N�����v����
	color_alpha_ = CLAMP(color_alpha_, 0.0f, 1.1f);
	//���l���ő�l�܂��͍ŏ��l�ɒB������I���t���O��true
	if (color_alpha_ >= 1.1f || color_alpha_ <= 0.0f) {
		end_ = true;
	}
}

//�`��
void Fade::draw() const {
	GScolor color{ 0,0,0,color_alpha_ };
	GSvector2 scale{ 10.0f,6.0f };
	gsDrawSprite2D(Texture_Fade, NULL, NULL, NULL, &color, &scale, NULL);
}

//�t�F�[�h���I����������擾
bool Fade::is_end() const{
	return end_;
}

//�t�F�[�h�C���܂��̓t�F�[�h�A�E�g�ɐ؂�ւ�
void Fade::change_fade_flg() {
	end_ = false;
	fade_in_ = !fade_in_;
}
