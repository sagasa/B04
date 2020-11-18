#include "Player.h"
#include "Assets.h"
#include "IWorld.h"
#include"Field.h"
#include <iostream>


// �ړ��͈�
const float MovingRangeX = 100.0f;
const float MovingRangeY = 70.0f;

const float Velocity = 1;

// �R���X�g���N�^
Player::Player(IWorld* world, const GSvector3& position) :mesh_(Mesh_Poltergeist, Mesh_CarGhost, Animation_CarGhost){
    world_ = world;
    name_ = "Player";
    tag_ = "PlayerTag";
    transform_.position(position);
    collider_ = BoundingSphere{ 0.75f };
}

const GSvector3 gravity{ 0.0f, 0.02f, 0.0f };
// �X�V
void Player::update(float delta_time) {
    //�d�́H
    velocity_ -= gravity * delta_time;
        velocity_ *= 0.9f;

    // �L�[�{�[�h�̓��͂���ړ��ʂ����߂�
    GSvector3 inputVelocity{ 0.0f, 0.0f, 0.0f };
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
    // �ړ��ʂ��v�Z
    inputVelocity.normalize();
    float speed = 0.02f;    // �ړ��X�s�[�h
    velocity_ += inputVelocity *delta_time* speed;
    velocity_.x = CLAMP(velocity_.x, -Velocity, Velocity);
    if (gsGetKeyTrigger(GKEY_SPACE) == GS_TRUE&& on_ground_) {
        velocity_.y = 0.3f;
        on_ground_ = false;
      //  std::cout << "Jump" << std::endl;
    }
    // ���W���擾
    GSvector3 position = transform_.position();
    // ���W���X�V
    position += velocity_;
    // ��ʊO�ɏo�Ȃ��悤�Ɉړ��͈͂𐧌�����
    position.x = CLAMP(position.x, -MovingRangeX, MovingRangeX);
    position.y = CLAMP(position.y, -MovingRangeY, MovingRangeY);
    // ���W�̐ݒ�
    transform_.position(position);

   
    GSvector3 intersect;//�n�ʂƂ̌�_
	BoundingSphere sphere = collider();
    if (world_->field()->collide(sphere, &intersect)) {
        //���������_����y���W�̂ݕ␳����
    }
    if (world_->field()->collide(Line{ sphere .center,sphere.center+GSvector2{0,-sphere.radius}}, &intersect)) {
        //���������_����y���W�̂ݕ␳����
        position.y = intersect.y+ sphere.radius;
        on_ground_ = true;
        transform_.position(position);
    }

    //���[�V�����ύX
    mesh_.change_motion(motion_);
    //���b�V���̍X�V
    mesh_.update(delta_time);
    //�s���ݒ�
    mesh_.transform(transform_.localToWorldMatrix());
}


// �`��
void Player::draw() const {
    glPushMatrix();
    mesh_.draw();
    glMultMatrixf(transform_.localToWorldMatrix());
    //gsDrawMesh(Mesh_Player);
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    //collider_.draw();
    glPopAttrib();


    glPopMatrix();
}

// �Փˏ���
void Player::react(Actor& other) {
    // �G�ƏՓ˂����ꍇ�͎��S
    if (other.tag() == "EnemyTag" || other.tag() == "EnemyBulletTag") {
        //die();
    }
}