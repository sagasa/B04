#pragma once
#include"Actor.h"
#include "World.h"
class ClearObj :public Actor {
public:
    //�R���X�g���N�^
    ClearObj(IWorld* world, const GSvector3& position)
    {
        world_ = world;
        name_ = "ClearFlag";
        tag_ = "ClearTag";
        float radius = 7;
        GSvector3 center{position.x-radius/2,position.y-radius/2,0};
        collider_ = BoundingSphere{ radius ,center};
        enable_collider_ = true;
    }
    // �`��
    void draw() const override
    {
        collider().draw();
    }

    void react(Actor& other) override
    {
        if (other.tag() == "PlayerTag")
        {
            world_->game_clear();
        }
    }
};

