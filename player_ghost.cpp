#include "player_ghost.h"

#include <iostream>

#include "IWorld.h"
#include "AttackCollider.h"


void player_ghost::wake_up()
{
}

void player_ghost::stop()
{
}

void player_ghost::change_state(State state, GSuint motion, bool loop)
{
    state_ = state;
    motion_loop_ = loop;
    mesh_.change_motion(motion, loop);
}

void player_ghost::attack()
{
    // 攻撃判定を出現させる場所の距離
    const float AttackColliderDistance{ 0.5f };
    // 攻撃判定の半径
    const float AttackColliderRadius{ 0.3f };
    // 攻撃判定を出す場所の高さ
    const float AttackColliderHeight{ 1.0f };

    // 攻撃判定が有効になるまでの遅延時間
    const float AttackCollideDelay{ 15.0f };
    // 攻撃判定の寿命
    const float AttackCollideLifeSpan{ 5.0f };

    // 衝突判定を出現させる座標を求める（前方の位置）
    GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
    // 高さの補正（足元からの高さ）
    position.y += AttackColliderHeight;
    // 衝突判定用の球を作成
    BoundingSphere collider{ AttackColliderRadius, position };
    // 衝突判定を出現させる
    world_->add_actor(new AttackCollider{ world_, collider,
        "PlayerAttackTag", "PlayerAttack", AttackCollideLifeSpan, AttackCollideDelay });

    change_state(Attack, 2, false);

}

void player_ghost::update_attack()
{
}


bool player_ghost::on_hit(const Actor& attacker, float atk_power)
{
    std::cout << "Hit\n";
}

player_ghost::player_ghost(IWorld* world, const GSvector3& position):Player(world,position, AnimatedMesh{ Mesh_Player, Skeleton_Player, Animation_Player })
{
    motion_ = 0;
    mesh_.change_motion(motion_);
}


const float Velocity = 0.15f;
const GSvector3 gravity{ 0.0f, 0.01f, 0.0f };

void player_ghost::update(float delta_time)
{
	switch (state_)
	{
    case Idle:
        //攻撃開始
        if (gsGetKeyState(GKEY_F) == GS_TRUE)
            attack();
        break;
    case Attack:
        update_attack();
        break;
    case Damage:

        break;
	}
	//デフォ以外なら
	if(state_!=Idle)
	{
        state_timer_ += delta_time;
        if(mesh_.motion_end_time() <= state_timer_)
        {
            if (motion_loop_)
                state_timer_ = 0;
            else
                change_state(Idle, 0);
        }
	}
	
    //重力？
	if(0<gravity_timer_)
	{
		gravity_timer_ -= delta_time;
	}
    velocity_ -= gravity * delta_time * (1-CLAMP(gravity_timer_, 0, 30) / 30);
    velocity_ *= 0.9f;

    GSvector2 inputVelocity = get_input();
    if (inputVelocity.x < 0) {
        transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
    }
    else if (inputVelocity.x > 0) {
        transform_.localRotation(GSquaternion::euler(0.0f, 90.0f, 0.0f));
    }
    
	if(0<inputVelocity.y)
	{
        //上に移動したなら
        gravity_timer_ = 60;
	}else if(inputVelocity.y<0)
	{
        //下入力で重力を適応
        gravity_timer_ = 0;
	}
	
    //地上で下入力を許可しない
    if (on_ground_)
    {
        inputVelocity.y = MAX(inputVelocity.y, 0);
    }
	// 移動量を計算
	float speed = 0.04f;    // 移動スピード
    velocity_ += inputVelocity * delta_time * speed;
    velocity_.x = CLAMP(velocity_.x, -Velocity, Velocity);

    update_physics(delta_time);
	
    //モーション変更
    
    
    //メッシュの更新
    mesh_.update(delta_time);
    //行列を設定
    mesh_.transform(transform().localToWorldMatrix());
}
