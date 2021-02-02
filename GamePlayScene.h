#ifndef GAME_PLAY_SCENE_H_
#define GAME_PLAY_SCENE_H_

#include"IScene.h"
#include"World.h"
#include"Fade.h"

//�Q�[���v���C�V�[��
class GamePlayScene :public IScene {
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
	//���[���h�N���X
	World world_;
	//�I���t���O
	bool is_end_{ false };
	//���̃V�[��
	std::string next_scene_{"SceneNull"};
	//�t�F�[�h�N���X
	Fade fade_;
	//�X�e�[�W�����o�����߂̃^�C�}�[
	float draw_timer_;
};

#endif
