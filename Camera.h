#pragma once
#include "Actor.h"


// �J�����N���X
class Camera : public Actor {
public:
    // �R���X�g���N�^
    Camera(IWorld* world);
    //�X�V
    virtual void update(float delta_time)override;
    // �`��
    virtual void draw() const override;

private:
    Actor* player_;
};