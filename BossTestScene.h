#ifndef BOSS_TEST_SCENE
#define BOSS_TEST_SCENE

#include "IScene.h"
#include"World.h"
class BossTestScene :
	public IScene
{
	//�J�n
	virtual void start(int numer) override;
	//�X�V
	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() const override;
	//�I�����Ă��邩�H
	virtual bool is_end() const override;
	//���̎s0������Ԃ�
	virtual std::string next() const override;
	//���݂̃X�e�[�W�ԍ���Ԃ�
	virtual int stage_number() const override;
	//�I��
	virtual void end() override;

private:
	World world_;
	bool load_{ false };
};

#endif