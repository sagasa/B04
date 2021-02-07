#ifndef TITLE_SCENE_H_
#define TITLE_SCENE_H_

#include"IScene.h"
#include<gslib.h>
#include"World.h"
#include"Fade.h"
#include<vector>


//�^�C�g���V�[��
class TitleScene : public IScene {
public:

	//���
	enum class State {
		Push_Button,
		Select,
		Tutorial
	};

	//�J�n
	virtual void start(int number) override;
	//�X�V
	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() const override;
	//�I�����Ă��邩�H
	virtual bool is_end() const override;
	//���̃V�[����Ԃ�
	virtual std::string next() const override;
	//���݂̃X�e�[�W�ԍ���Ԃ�
	virtual int stage_number() const override;
	//�I��
	virtual void end() override;

private:
	void update_push_button(float delta_time);
	void update_select(float delta_time);
	void update_tutorial(float delta_time);
	//���l�̍X�V
	void update_alpha(int num, float detla_time);

private:

	//�I���t���O
	bool is_end_{ false };
	//���̃V�[��
	std::string nextScene_;
	//�t�F�[�h�N���X
	Fade fade_;
	//�{�^���\���p�^�C�}�[
	float button_timer_;
	//���
	State state_;
	//���l�̑����t���O
	bool alpha_flg_;
	std::vector<float> alphas_;
	int select_;
	std::vector<float>pos_x_;
	int num_;
	bool is_move_;
	float move_timer_;
	float alpha_;

};

#endif
