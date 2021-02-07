#pragma once
#include "Actor.h"
#include "DamageProp.h"
#include "AnimatedMesh.h"

// プレーヤ
class Player : public Actor, public DamageProp {
    

protected:
    int jump_count_;
    float jump_force_;
	
    bool on_ground_;
    //アニメーションメッシュ
    AnimatedMesh mesh_;
  
	//判定が硬いか
    bool is_soft_;

	//上下の高さ拡張
    float height_ext_ = 0;
	
    //速度を適応 フィールドとの衝突処理
    void update_physics(const float delta_time);
	
    //フィールドとの衝突処理
    void jump(const int count,const float force);

    //入力をvec2として取得
    GSvector2 static get_input();
public:	
    // コンストラクタ
    Player(IWorld* world, const GSvector3& position,const AnimatedMesh& mesh);

    virtual void wake_up(const int base_hp) = 0;

    virtual void stop() = 0;
};
