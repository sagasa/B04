#ifndef UI_H_
#define UI_H_

#include"Actor.h"
#include"ActorProp.h"

//UI�N���X
class UI : public Actor {
public:
	//�R���X�g���N�^
	UI() = default;
	//�f�X�g���N�^
	~UI() = default;
	// GUI�̕`��
	virtual void draw_gui() const override;

public:
	ActorProp* player_;
};


#endif
