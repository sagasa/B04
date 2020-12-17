#include "player_ghost.h"
#include "IWorld.h"
#include "AttackCollider.h"


player_ghost::player_ghost(IWorld* world, const GSvector3& position):Player(world,position, AnimatedMesh{ Mesh_Player, Skeleton_Player, Animation_Player })
{
    motion_ = 0;
}


const float Velocity = 0.15f;
const GSvector3 gravity{ 0.0f, 0.02f, 0.0f };

void player_ghost::update(float delta_time)
{
    //重力？
    velocity_ -= gravity * delta_time;
    velocity_ *= 0.9f;

    GSvector2 inputVelocity = get_input();
    if (inputVelocity.x < 0) {
        transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
    }
    else if (inputVelocity.x > 0) {
        transform_.localRotation(GSquaternion::euler(0.0f, 90.0f, 0.0f));
    }
    // 移動量を計算
    inputVelocity.normalize();
    float speed = 0.04f;    // 移動スピード
    velocity_ += inputVelocity * delta_time * speed;
    velocity_.x = CLAMP(velocity_.x, -Velocity, Velocity);

    if (gsGetKeyTrigger(GKEY_SPACE) == GS_TRUE ) {
        jump(5, 0.15f);
    }

    if (gsGetKeyState(GKEY_F) == GS_TRUE) {

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
    }

    update_physics(delta_time);
	
    //モーション変更
    mesh_.change_motion(motion_);
    //メッシュの更新
    mesh_.update(delta_time);
    //行列を設定
    mesh_.transform(transform().localToWorldMatrix());
}
