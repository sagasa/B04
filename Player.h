#pragma once
#include "Actor.h"
#include "AnimatedMesh.h"

// プレーヤ
class Player : public Actor {
    bool on_ground_;
    int jump_count_;
    //アニメーションメッシュ
    AnimatedMesh mesh_;
    //モーション番号
    GSuint motion_;
public:
    // コンストラクタ
    Player(IWorld* world, const GSvector3& position);
    // 更新
    virtual void update(float delta_time) override;
    // 描画
    virtual void draw() const override;
    // 衝突処理
    virtual void react(Actor& other) override;
};
