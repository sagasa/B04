#ifndef ENEMY_TEST_SCENE_H_
#define ENEMY_TEST_SCENE_H_

#include"IScene.h"
#include"World.h"


class EnemyTestScene : public IScene {
public:
	//�J�n
	void start(int number) override;
	//�X�V
	virtual void update(float delta_time)override;
	//�`��
	virtual void draw() const override;
	//�I�����Ă��邩�H
	virtual bool is_end()const override;
	//���̃V�[����Ԃ�
	virtual std::string next()const override;
	//���݂̃X�e�[�W�ԍ���Ԃ�
	virtual int stage_number() const override;
	//�I��
	virtual void end()override;

private:
	//�I���t���O
	bool is_end_{ false };
	//���[���h
	World world_;

};
#endif