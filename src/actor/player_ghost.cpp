#include "player_ghost.h"

#include <iostream>

#include "IWorld.h"
#include "AttackCollider.h"
#include "interact_collider.h"

void player_ghost::wake_up(const int base_hp)
{
	
}

void player_ghost::stop()
{
    die();
}


void player_ghost::change_state(State state, GSuint motion, bool loop)
{
    state_ = state;
    if (!mesh_.change_motion(motion, loop))return;
    motion_loop_ = loop;
    state_timer_ = 0;
}

void player_ghost::attack()
{
    // 攻撃判定を出現させる場所の距離
    const float AttackColliderDistance{ 1.4f };
    // 攻撃判定の半径
    const float AttackColliderRadius{ 0.9f };
    // 攻撃判定を出す場所の高さ
    const float AttackColliderHeight{ 1.0f };

    // 攻撃判定が有効になるまでの遅延時間
    const float AttackCollideDelay{ 5.0f };
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
        "PlayerAttack", "PlayerAttack",name_, AttackCollideLifeSpan, AttackCollideDelay });

    change_state(Attack, 2, false);

	//SE
    gsPlaySE(SE_P_Ghost_Attack);
}


bool player_ghost::on_hit(const Actor& attacker, float atk_power)
{
    //地形ダメージ
    if (attacker.tag() == "FieldDamage")
    {
        hp_ -= atk_power;
        bar_.set_HP(hp_);
        if (hp_ <= 0)
        {
            stop();
            change_state(Damage, 4, false);
            world_->game_over();
        }
        gsPlaySE(SE_GhostDamage);
        return true;
    }
	
	if((attacker.tag()=="EnemyAttack"|| attacker.tag() == "EnemyTag")&&state_!=Damage&&state_!=Attack)
	{
        hp_-=atk_power;
        bar_.set_HP(hp_);
		if(hp_<=0)
		{
            world_->game_over();
            change_state(Damage, 4, false);
		}else
			change_state(Damage, 3,false);
        std::cout << "Hit "<<hp_<<"\n";
        //SE
        gsPlaySE(SE_GhostDamage);
        return true;
	}
    return false;
}

player_ghost::player_ghost(IWorld* world, const GSvector3& position, const int hp):Player(world,position, AnimatedMesh{ Mesh_Player, Skeleton_Player, Animation_Player })
{
    change_state(Attack, 2,false);
    hp_ = hp;
    bar_.set_HP_NoAnim(hp_);
}

const GSvector2 hpOffset{5,5};
// 描画
void player_ghost::draw() const {
    bar_.draw(hpOffset);
    mesh_.draw();
    glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    collider_.draw();
    glPopAttrib();

    glPopMatrix();

    mesh_.draw();
    glPushMatrix();
    /*
	GSvector2 pos{ 0,0 };
    for (int i = 0; i < hp_; ++i)
    {
        GSvector2 scale{ 0.03f, 0.03f };
        gsDrawSprite2D(Texture_Hp, &pos, NULL, NULL, NULL, &scale, NULL);
        pos.x += 40;
    }*/
    glPopMatrix();
}


const float Velocity = 0.1f;
const GSvector3 gravity{ 0.0f, 0.01f, 0.0f };

void player_ghost::update(float delta_time)
{
    bar_.update(delta_time);
	
	//デフォ以外なら
	if(state_!=Idle)
	{
        state_timer_ += delta_time;
        if(mesh_.motion_end_time() <= state_timer_)
        {
            if (motion_loop_)
                state_timer_ = 0;
            else
            {
                std::cout << "Idle\n";
                change_state(Idle, 0);
            }
        }
	}else
	{
        //攻撃開始
        if (gsGetKeyState(GKEY_F) == GS_TRUE || gsXBoxPadButtonTrigger(0,GS_XBOX_PAD_X))
            attack();
	}
	
    //憑依
    if ((gsGetKeyTrigger(GKEY_E) || gsXBoxPadButtonTrigger(0,GS_XBOX_PAD_Y)) && state_ != Attack)
        world_->add_actor(new interact_collider{world_,collider(),tag_,name_,[this](Actor& actor)
        {
            dynamic_cast<Player&>(actor).wake_up(hp_);
            stop();
        }});
    //重力？
	if(0<gravity_timer_)
	{
		gravity_timer_ -= delta_time;
	}
    velocity_ -= gravity * delta_time * (1-CLAMP(gravity_timer_, 0, 30) / 30);
    velocity_ *= 0.9f;

	//生きている間
	if(0<hp_)
	{
        GSvector2 inputVelocity = get_input();
        if (inputVelocity.x < 0) {
            transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
        }
        else if (inputVelocity.x > 0) {
            transform_.localRotation(GSquaternion::euler(0.0f, 90.0f, 0.0f));
        }

        if (0 < inputVelocity.y)
        {
            //上に移動したなら
            gravity_timer_ = 60;
        }
        else if (inputVelocity.y < 0)
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
        velocity_.y = CLAMP(velocity_.y, -Velocity, Velocity);
	}
    

    update_physics(delta_time);
    
    //メッシュの更新
    mesh_.update(delta_time);
    //行列を設定
    mesh_.transform(transform().localToWorldMatrix());
}
