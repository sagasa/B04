#ifndef RESULT_SCENE_H_
#define RESULT_SCENE_H_

#include"IScene.h"
#include"World.h"
#include"Fade.h"
#include<vector>

class ResultScene : public IScene
{
public:

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
	//���l�̍X�V
	void update_alpha(int num, float detla_time);
	//�L�[���́A�p�b�h�̓��͏���
	void input();
	//�{�^���̕`��
	void draw_button() const;

private:
	//���[���h�N���X
	World world_;
	//�I���t���O
	bool is_end_{ false };
	//�e�e�N�X�`���̃��l�̃f�[�^
	std::vector<float> alphas_;
	//�z�񎯕ʔԍ�
	int num_;
	//���l�̑����t���O
	bool alpha_flg_;
	//���̃V�[��
	std::string nextScene_;
	//�t�F�[�h�N���X
	Fade fade_;
	//�^�C�}�[
	float timer_;
};

#endif // !RESULT_SCENE_H_