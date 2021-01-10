#ifndef FADE_H_
#define FADE_H_

#include <gslib.h>

//�t�F�[�h�N���X
class Fade {
public:
	//�R���X�g���N�^
	Fade() = default;
	//�f�X�g���N�^
	~Fade();
	//�J�n(�t�F�[�h�C���Ȃ�true,�t�F�[�h�A�E�g�Ȃ�false),�������Ƀt�F�[�h�C��or�A�E�g���I���܂ł̎��Ԏ���
	void start(bool fade_in,float fade_time);
	//�X�V
	void update(float delta_time);
	//�`��
	void draw() const;
	//�t�F�[�h���I����������擾
	bool is_end() const;
	//�t�F�[�h�C���܂��̓t�F�[�h�A�E�g�ɐ؂�ւ�
	void change_fade_flg();

private:
	//���l
	float color_alpha_;
	//���l�ɓ����l
	float alpha_value_;
	//�t�F�[�h�C�����t�F�[�h�A�E�g���H
	bool fade_in_;
	//�t�F�[�h���I��������H
	bool end_;
	//�N���b�N���ꂽ���H
	bool change_fade_flg_;

};


#endif // !FADE_H_