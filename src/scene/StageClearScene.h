#ifndef STAGE_CLEAR_SCENE
#define STAGE_CLEAR_SCENE

#include"IScene.h"
#include"World.h"
#include"Fade.h"


class StageClearScene : public IScene
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
	//�����̕`��
	void draw_number() const;
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
	//�X�e�[�W�����o�����߂̃^�C�}�[
	float draw_timer_;
	//�X�e�[�W�ԍ���ۑ�����ϐ�
	int static_stage_number_;

};



#endif // !STAGE_CLEAR_SCENE



