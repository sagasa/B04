#include "Player.h"
#include "Assets.h"
#include "IWorld.h"


// �ړ��͈�
const float MovingRangeX = 100.0f;
const float MovingRangeY = 70.0f;

const float Velocity = 1;

// �R���X�g���N�^
Player::Player(IWorld* world, const GSvector3& position) {
    world_ = world;
    name_ = "Player";
    tag_ = "PlayerTag";
    transform_.position(position);
    collider_ = BoundingSphere{ 5.0f };
}

const GSvector3 gravity{ 0.0f, 0.2f, 0.0f };
// �X�V
void Player::update(float delta_time) {
    //�d�́H
    velocity_ -= gravity * delta_time;
        velocity_ *= 0.9;

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
    float speed = 1.0f;    // �ړ��X�s�[�h
    //velocity_ = inputVelocity.getNormalized() * speed * delta_time;
    velocity_ += inputVelocity *delta_time;
    velocity_.x = CLAMP(velocity_.x, -Velocity, Velocity);

    // ���W���擾
    GSvector3 position = transform_.position();
    // ���W���X�V
    position += velocity_;
    // ��ʊO�ɏo�Ȃ��悤�Ɉړ��͈͂𐧌�����
    position.x = CLAMP(position.x, -MovingRangeX, MovingRangeX);
    position.y = CLAMP(position.y, -MovingRangeY, MovingRangeY);
    // ���W�̐ݒ�
    transform_.position(position);

}


// �`��
void Player::draw() const {
    glPushMatrix();
    glMultMatrixf(transform_.localToWorldMatrix());
    gsDrawMesh(Mesh_Player);

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