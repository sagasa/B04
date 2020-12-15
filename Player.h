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

// プレーヤ
class Player : public Actor, public HP {
    bool on_ground_;
    int jump_count_;
    player_info& info;
    
public:
    
    // コンストラクタ
    Player(IWorld* world, const GSvector3& position);
    // 更新
    void update(float delta_time) override;
    // 描画
    void draw() const override;
    // 衝突処理
    void react(Actor& other) override;
};
