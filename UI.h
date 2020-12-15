#ifndef UI_H_
#define UI_H_

#include"Actor.h"
#include"ActorProp.h"

//UI�N���X
class UI : public Actor {
public:
	//�R���X�g���N�^
	UI(IWorld* world);
	//�A�b�v�f�[�g
	void update(float delta_time) override;
	// GUI�̕`��
	virtual void draw_gui() const override;

private:
	void change_gui();

private:
	Actor* player_{nullptr};
	int hp_{ 0 };
	int shield_hp_{ 0 };
};


#endif
