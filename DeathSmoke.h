#ifndef DEATH_SMOKE_H_
#define DEATH_SMOKE_H_

#include"Actor.h"
#include"DamageProp.h"

class DeathSmoke : public Actor {
public:

	//�R���X�g���N�^
	DeathSmoke(IWorld* world, const GSvector3& position);
	//�X�V
	virtual void update(float delta_time) override;
	//�`��
	virtual void draw() const override;
	//�Փ˃��A�N�V����
	virtual void react(Actor& other) override;

private:
	//��ʊO���H
	bool is_out_camera() const;
	//�ǂƂ̏Փ˔���
	void collide_field();
	
};


#endif