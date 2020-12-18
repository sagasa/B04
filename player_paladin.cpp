#include "player_paladin.h"
#include "IWorld.h"
#include <iostream>


#include "AttackCollider.h"
#include "player_ghost.h"


void player_paladin::wake_up()
{
    hp_ = 3;
    tag_ = "PlayerTag";
    name_ = "Player";
    change_state(Jump, 1,false);
}

void player_paladin::stop()
{
    tag_ = "PlayerEquip";
	name_ = "Equip";
    change_state(Stop, 0);
}

bool player_paladin::can_interact(const Actor& from)
{
    return true;
}

void player_paladin::change_state(State state, GSuint motion, bool loop)
{
    state_ = state;
    motion_loop_ = loop;
    state_timer_ = 0;
    mesh_.change_motion(motion, loop);
}

void player_paladin::attack()
{
    // �U��������o��������ꏊ�̋���
    const float AttackColliderDistance{ 1.2f };
    // �U������̔��a
    const float AttackColliderRadius{ 1.0f };
    // �U��������o���ꏊ�̍���
    const float AttackColliderHeight{ 1.0f };

    // �U�����肪�L���ɂȂ�܂ł̒x������
    const float AttackCollideDelay{ 15.0f };
    // �U������̎���
    const float AttackCollideLifeSpan{ 15.0f };

    // �Փ˔�����o����������W�����߂�i�O���̈ʒu�j
    GSvector3 position = transform_.position() + transform_.forward() * AttackColliderDistance;
    // �����̕␳�i��������̍����j
    position.y += AttackColliderHeight;
    // �Փ˔���p�̋����쐬
    BoundingSphere collider{ AttackColliderRadius, position };
    // �Փ˔�����o��������
    world_->add_actor(new AttackCollider{ world_, collider,
        "PlayerAttack", "PlayerAttack", AttackCollideLifeSpan, AttackCollideDelay ,3});

    change_state(Attack, 8, false);
}


player_paladin::player_paladin(IWorld* world, const GSvector3& position) :Player(world, position, AnimatedMesh{ Mesh_Paladin, Skeleton_Paladin, Animation_Paladin })
{
    is_soft_ = false;
    stop();
}

bool player_paladin::on_hit(const Actor& attacker, float atk_power)
{
    if (state_ == Stop) return false;
    if ((attacker.tag() == "EnemyAttack" || attacker.tag() == "EnemyTag") && state_ != Damage)
    {
        hp_ -= atk_power;
        if (hp_ <= 0)
        {
            stop();
            world_->add_actor(new player_ghost{ world_,transform_.position() });
            std::cout << "Stop " << hp_ << "\n";
        }else
        {
            change_state(Damage, 5, false);
            std::cout << "Hit paladin " << hp_<<" "<<transform_.position().z << "\n";
        }
        return true;
    }
    return false;
}

const float Velocity = 0.07f;
const GSvector3 gravity{ 0.0f, 0.01f, 0.0f };

void player_paladin::update(float delta_time)
{
    //�d��
    velocity_ -= gravity * delta_time;
    //�N��
    if (state_ != Stop) {

        GSvector2 inputVelocity = get_input();
        inputVelocity.y = 0;
    	
        //�U�����͓��͂��΂�
        if (state_ == Attack)
        {
            inputVelocity.x = 0;
        }

        if (inputVelocity.x < 0) {
            transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
        }
        else if (inputVelocity.x > 0) {
            transform_.localRotation(GSquaternion::euler(0.0f, 90.0f, 0.0f));
        }
        else
        {
            velocity_.x *= 0.9f;
        }

        if (state_ != Attack && state_ != Damage)
        {
            //�U���J�n
            if (gsGetKeyState(GKEY_F))
                attack();
        }

        if (state_ != Attack && gsGetKeyTrigger(GKEY_E))
        {
            stop();
            world_->add_actor(new player_ghost{ world_,transform_.position() });
        }
    	

        //�W�����v�L�����Z��
        if (state_ == Jump && on_ground_ && jump_count_ <= 0)
            change_state(Idle, 2);
        //�f�t�H�ȊO�Ȃ�
        if (state_ != Idle && state_ != Move)
        {
            state_timer_ += delta_time;
            if (mesh_.motion_end_time() <= state_timer_)
            {
                if (motion_loop_)
                    state_timer_ = 0;
                else
                {
                    change_state(Idle, 2);
                }
            }

        }
        else
        {
            //���x�ŃA�j���[�V�����ύX
            if (inputVelocity.x != 0)
            {
                change_state(Move, 9);
            }
            else
            {
                change_state(Move, 2);
            }
            //�U���J�n
            if (gsGetKeyState(GKEY_F) == GS_TRUE)
                attack();
        }

        //�n��ŉ����͂������Ȃ�
        if (on_ground_)
        {
            if (gsGetKeyState(GKEY_SPACE))
            {
                jump(3, 0.7);
                change_state(Jump, 6, false);
            }
            inputVelocity.y = MAX(inputVelocity.y, 0);
        }
        // �ړ��ʂ��v�Z
        float speed = 0.04f;    // �ړ��X�s�[�h
        velocity_ += inputVelocity * delta_time * speed;
        velocity_.x = CLAMP(velocity_.x, -Velocity, Velocity);
        velocity_.y = CLAMP(velocity_.y, -Velocity, Velocity);
    	
    }else
    {
        velocity_ *= 0.9;
    }
    update_physics(delta_time);
	
    //���b�V���̍X�V
    mesh_.update(delta_time);
    //�s���ݒ�
    mesh_.transform(transform().localToWorldMatrix());
}
