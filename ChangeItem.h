#pragma once
#include "Actor.h"

class ChangeItem:Actor
{
    //�R���X�g���N�^
    ChangeItem(IWorld* world, const GSvector3& position);
    // �X�V
    void update(float delta_time) override;
    // �`��
    void draw() const override;
};