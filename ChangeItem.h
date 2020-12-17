#pragma once
#include "Actor.h"

class ChangeItem:Actor
{
    //コンストラクタ
    ChangeItem(IWorld* world, const GSvector3& position);
    // 更新
    void update(float delta_time) override;
    // 描画
    void draw() const override;
};