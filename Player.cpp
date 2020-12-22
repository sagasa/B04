#include "Player.h"
#include "IWorld.h"
#include"Field.h"
#include <iostream>
#include "AttackCollider.h"

void Player::update_physics(const float delta_time)
{
    // ���W���X�V
    transform_.translate(velocity_.xy * delta_time, GStransform::Space::World);
	
    //�n�ʂƂ̏Փ˔���(�����Ƃ̌�������)
    BoundingSphere sphere = collider();
    //�W�����v�\��
    on_ground_ = world_->field()->collide(Line{ sphere.center,sphere.center + GSvector2{0,-sphere.radius} });
	
    if (world_->field()->collide(Line{ sphere.center,sphere.center + GSvector2{0,sphere.radius} })) {
        //�W�����v��~
        jump_count_ = 0;
    }

    //�ǂƂ̏Փ˔���(���̂Ƃ̔���)
    GSvector3 center;
    //�Փˌ�̋��̂̒��S���W
    if (world_->field()->collide(sphere, &center)) {
        //���W�n��߂�
        center -= collider_.center;
        if (is_soft_)
        {
        	//�S�[�X�g���̂ӂ�ӂ�
            GSvector3 vel = (center - transform_.position());
            velocity_ += vel * delta_time * 0.2f;
            velocity_.z = 0;
        }else
        {
            //�����߂�
            transform_.position(center.xy);
        }
    }

	//�W�����v�X�V
    if (0 < jump_count_)
    {
        jump_count_--;
        velocity_.y += jump_force_;
    }
}

void Player::jump(const int count, const float force)
{
	if(on_ground_)
	{
        velocity_.y = MIN(0, velocity_.y);
        jump_count_ = count;
        jump_force_ = force;
	}
}

GSvector2 Player::get_input()
{
    // �L�[�{�[�h�̓��͂���ړ��ʂ����߂�
    GSvector2 inputVelocity{0,0};
    if (gsGetKeyState(GKEY_LEFT) == GS_TRUE) {
        inputVelocity.x = -1.0f;
    }
    if (gsGetKeyState(GKEY_RIGHT) == GS_TRUE) {
        inputVelocity.x = 1.0f;
    }
    if (gsGetKeyState(GKEY_UP) == GS_TRUE) {
        inputVelocity.y = 1.0f;
    }
    if (gsGetKeyState(GKEY_DOWN) == GS_TRUE) {
        inputVelocity.y = -1.0f;
    }
    return inputVelocity.normalized();
}

// �R���X�g���N�^
Player::Player(IWorld* world, const GSvector3& position, const AnimatedMesh& mesh) :mesh_{ mesh }{//�ǂ�����Ⴂ���̃�
    world_ = world;
    name_ = "Player";
    tag_ = "PlayerTag";
    transform_.position(position);
    collider_ = BoundingSphere{ 0.8f,GSvector3{0.0f,0.8f,0.0f} };
    transform_.localRotation(GSquaternion::euler( 0.0f, 90.0f, 0.0f ));
}

// �`��
void Player::draw() const {
    mesh_.draw();
	glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    //gsDrawMesh(Mesh_Player);
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    collider_.draw();
    glPopAttrib();


    glPopMatrix();
}