#pragma once
#include"Actor.h"
#include "World.h"
class ClearObj :public Actor {
public:
    //コンストラクタ
    ClearObj(IWorld* world, const GSvector3& position)
    {
        world_ = world;
        name_ = "ClearFlag";
        tag_ = "ClearTag";
        collider_ = BoundingSphere{ 2 };
        enable_collider_ = true;
    }
    // 描画
    void draw() const override
    {
        collider().draw();
    }

    void collide(Actor& other) override
    {
        if (other.tag() == "PlayerTag")
        {
            world_->game_clear();
        }
    }
};

