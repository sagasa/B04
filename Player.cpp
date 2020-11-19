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
    collider_ = BoundingSphere{ 0.5f,GSvector3{0.0f,1.0f,0.0f} };
    transform_.localRotation(GSquaternion::euler( 0.0f, 90.0f, 0.0f ));
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
    if (inputVelocity.x < 0) {
        transform_.localRotation(GSquaternion::euler(0.0f, -90.0f, 0.0f));
    }
    else if(inputVelocity.x > 0){
        transform_.localRotation(GSquaternion::euler(0.0f, 90.0f, 0.0f));
    }
    // �ړ��ʂ��v�Z
    inputVelocity.normalize();
    float speed = 0.02f;    // �ړ��X�s�[�h
    velocity_ += inputVelocity *delta_time* speed;
    velocity_.x = CLAMP(velocity_.x, -Velocity, Velocity);
    if (gsGetKeyTrigger(GKEY_SPACE) == GS_TRUE&& on_ground_) {
        velocity_.y = 0.5f;
        on_ground_ = false;
      //  std::cout << "Jump" << std::endl;
    }
    // ���W���擾
    //GSvector3 position = transform_.position();
    // ���W���X�V
    //position += velocity_;
    // ��ʊO�ɏo�Ȃ��悤�Ɉړ��͈͂𐧌�����
    //position.x = CLAMP(position.x, -MovingRangeX, MovingRangeX);
    //position.y = CLAMP(position.y, -MovingRangeY, MovingRangeY);
    // ���W�̐ݒ�
    //transform_.position(position);
    transform_.translate(velocity_ * delta_time, GStransform::Space::World);

   
    /*GSvector3 intersect;//�n�ʂƂ̌�_
	BoundingSphere sphere = collider();
    if (world_->field()->collide(sphere, &intersect)) {
        //���������_����y���W�̂ݕ␳����
        intersect.y = position.y;
        transform_.position(intersect);
    }
    if (world_->field()->collide(Line{ position + sphere .center,sphere.center+GSvector2{0,-sphere.radius}}, &intersect)) {
        //���������_����y���W�̂ݕ␳����
        position.y = intersect.y;
        on_ground_ = true;
        transform_.position(position);
        velocity_.y = 0.0f;
    }*/

    //�ǂƂ̏Փ˔���(���̂Ƃ̔���)
    GSvector3 center;
    //�Փˌ�̋��̂̒��S���W
    if (world_->field()->collide(collider(), &center)) {
        //y���W�͕ύX���Ȃ�
        center.y = transform_.position().y;
        //�␳��̍��W�ɕύX����
        transform_.position(center);
    }
    //�n�ʂƂ̏Փ˔���(�����Ƃ̌�������)
    GSvector3 position = transform_.position();
    Line line;
    line.start = position + collider_.center;
    line.end = position + GSvector3{ 0.0f,-0.1f,0.0f };
    GSvector3 intersect;//�n�ʂƂ̌�_
    if (world_->field()->collide(line, &intersect)) {
        //��_�̈ʒu����y���W�̂ݕ␳����
        position.y = intersect.y;
        //���W��ύX����
        transform_.position(position);
        //�d�͂�����������
        velocity_.y = 0.0f;
        on_ground_ = true;
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
    collider_.draw();
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