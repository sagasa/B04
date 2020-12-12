#pragma once
#include "Actor.h"
#include "AnimatedMesh.h"
#include "player_info.h"

class HP
{
public:
    int HP_ = 3;
    void attack_from();
};

// �v���[��
class Player : public Actor, public HP {
    bool on_ground_;
    int jump_count_;
    player_info& info;
    
public:
    
    // �R���X�g���N�^
    Player(IWorld* world, const GSvector3& position);
    // �X�V
    void update(float delta_time) override;
    // �`��
    void draw() const override;
    // �Փˏ���
    void react(Actor& other) override;
};
