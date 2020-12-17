#pragma once
#include "Actor.h"
#include "ActorProp.h"
#include "AnimatedMesh.h"

// プレーヤ
class Player : public Actor, public ActorProp {
    int jump_count_;
    float jump_force_;

protected:
    bool on_ground_;
    //アニメーションメッシュ
    AnimatedMesh mesh_;
    //モーション番号
    GSuint motion_;
    //モーションのループ指定
    bool motion_loop_;
	//判定が硬いか
    bool is_soft_;
	
    //速度を適応 フィールドとの衝突処理
    void update_physics(const float delta_time);
	
    //フィールドとの衝突処理
    void jump(const int count,const float force);

    //入力をvec2として取得
    GSvector2 static get_input();
public:	
    // コンストラクタ
    Player(IWorld* world, const GSvector3& position,const AnimatedMesh& mesh);
    // 描画
    void draw() const override;

    virtual void wake_up() = 0;

    virtual void stop() = 0;
};
