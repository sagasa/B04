#pragma once
#include "PhysicsActor.h"
// �v���[��
class Player : public PhysicsActor {
public:
    // �R���X�g���N�^
    Player(IWorld* world, const GSvector3& position);
    // �X�V
    virtual void update(float delta_time) override;
    // �`��
    virtual void draw() const override;
    // �Փˏ���
    virtual void react(Actor& other) override;
};
