#pragma once
#include "Actor.h"
#include "AnimatedMesh.h"

// �v���[��
class Player : public Actor {
    bool on_ground_;
    int jump_count_;
    //�A�j���[�V�������b�V��
    AnimatedMesh mesh_;
    //���[�V�����ԍ�
    GSuint motion_;
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
