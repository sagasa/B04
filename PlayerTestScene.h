#ifndef PLAYER_TEST_SCENE_H_
#define PLAYER_TEST_SCENE_H_

#include "IScene.h"
#include"World.h"
class PlayerTestScene :
	public IScene
{
	void load(resource_loader& loader) override;
	//�J�n
	virtual void start() override;
	//�X�V
	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() const override;
	//�I�����Ă��邩�H
	virtual bool is_end() const override;
	//���̎s0������Ԃ�
	virtual std::string next() const override;
	//�I��
	virtual void end() override;

private:
	//�I���t���O
	bool is_end_{ false };
	World world_;
};

#endif

