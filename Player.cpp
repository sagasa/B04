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
    
    if (world_->field()->collide(Line{ sphere.center,sphere.center + GSvector2{0,sphere.radius+ height_ext_} })) {
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
            // ���������_����y���W�̂ݕ␳����
            center.y = transform_.position().y;
        	transform_.position(center.xy);
        }
    }
    if (!is_soft_)
    {
        // �n�ʂƂ̏Փ˔���i���C�Ƃ̌�������j
        GSvector3 position = transform_.position();
        GSvector3 intersect;  // �n�ʂƂ̌�_
        //�W�����v�\��
    	if (on_ground_ =world_->field()->collide(Line{ sphere.center,sphere.center + GSvector2{0,-sphere.radius - height_ext_} }, &intersect)) {
            // ���������_����y���W�̂ݕ␳����
            position.y = intersect.y+height_ext_-(collider_.center.y- sphere.radius);
            // ���W��ύX����
            transform_.position(position);
            velocity_.y = MAX(velocity_.y,0);
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
    //���X�e�B�b�N�̃x�N�g���l
    GSvector2 vector_stick = GSvector2::zero();
    //���X�e�B�b�N�̓��͂��擾
    gsXBoxPadGetLeftAxis(0, &vector_stick);
    // �L�[�{�[�h�̓��͂���ړ��ʂ����߂�
    GSvector2 inputVelocity{0,0};
    if (gsGetKeyState(GKEY_LEFT) == GS_TRUE || vector_stick.x <= -0.5f || gsXBoxPadButtonState(0,GS_XBOX_PAD_LEFT)) {
        inputVelocity.x = -1.0f;
    }
    if (gsGetKeyState(GKEY_RIGHT) == GS_TRUE || vector_stick.x >= 0.5f || gsXBoxPadButtonState(0, GS_XBOX_PAD_RIGHT)) {
        inputVelocity.x = 1.0f;
    }
    if (gsGetKeyState(GKEY_UP) == GS_TRUE || vector_stick.y >= 0.5f || gsXBoxPadButtonState(0, GS_XBOX_PAD_UP)) {
        inputVelocity.y = 1.0f;
    }
    if (gsGetKeyState(GKEY_DOWN) == GS_TRUE || vector_stick.y <= -0.5f || gsXBoxPadButtonState(0, GS_XBOX_PAD_DOWN)) {
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
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    collider_.draw();
    glPopAttrib();

    glPopMatrix();

    mesh_.draw();
    glPushMatrix();
    GSvector2 pos{ 0,0 };
    for (int i = 0; i < hp_; ++i)
    {
        gsDrawSprite2D(Texture_Hp, &pos, NULL, NULL, NULL, &GSvector2{ 0.03f, 0.03f }, NULL);
        pos.x += 40;
    }
    glPopMatrix();
}