#pragma once


#include <vector>
#include <functional>
#include"Actor.h"
#include "interact_prop.h"

//���씻��N���X
class interact_collider : public Actor {
public:
	//�R���X�g���N�^
	interact_collider(IWorld* world, const BoundingSphere& collider,
		const std::string& tag, const std::string& name, std::function<void(Actor&)> func);
	//�X�V
	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() const override;
	//�Փ˃��A�N�V����
	virtual void react(Actor& other) override;

private:
	// ���t���[���X�V
	std::vector<Actor*> cash_;
	std::function<void(Actor&)> func_;
	int count_ = 0;
};

