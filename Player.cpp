#include "Player.h"
#include "Assets.h"
#include "IWorld.h"
#include"Field.h"
#include <iostream>


#include "player_ghost.h"
#include "player_paladin.h"


// �ړ��͈�
const float MovingRangeX = 100.0f;
const float MovingRangeY = 70.0f;

const float Velocity = 0.15f;

static player_paladin Paladin =player_paladin{};
static player_ghost Ghost = player_ghost{};

// �R���X�g���N�^
Player::Player(IWorld* world, const GSvector3& position) :info(Paladin){//�ǂ�����Ⴂ���̃�
    world_ = world;
    name_ = "Player";
    tag_ = "PlayerTag";
    transform_.position(position);
    collider_ = BoundingSphere{ 0.8f,GSvector3{0.0f,0.8f,0.0f} };
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
    float speed = 0.04f;    // �ړ��X�s�[�h
    velocity_ += inputVelocity *delta_time* speed;
    velocity_.x = CLAMP(velocity_.x, -Velocity, Velocity);
    if (gsGetKeyTrigger(GKEY_SPACE) == GS_TRUE&& on_ground_) {
        jump_count_ = 5;
        velocity_.y = MIN(0, velocity_.y);
        on_ground_ = false;
      //  std::cout << "Jump" << std::endl;
    }
	if(0<jump_count_)
	{
        jump_count_--;
        velocity_.y += 0.1f;
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

   
    {
        //�n�ʂƂ̏Փ˔���(�����Ƃ̌�������)
        BoundingSphere sphere = collider();
        if (world_->field()->collide(Line{ sphere.center,sphere.center + GSvector2{0,-sphere.radius} })) {
			//�W�����v�\��
            on_ground_ = true;
        }
        if (world_->field()->collide(Line{ sphere.center,sphere.center + GSvector2{0,sphere.radius} })) {
            //�W�����v��~
            jump_count_ = 0;
        }
    }
    
    //�ǂƂ̏Փ˔���(���̂Ƃ̔���)
    {
        BoundingSphere sphere = collider();
        GSvector3 center;
        //�Փˌ�̋��̂̒��S���W
        if (world_->field()->collide(sphere, &center)) {
            

            //���W�n��߂�
            center-= collider_.center;
            GSvector3 vel = (center - transform_.position());
    	
            velocity_+= vel*delta_time*0.2f;
            velocity_.z = 0;

        	//�␳��̍��W�ɕύX����
           // transform_.position(center);
        }
    }
    
    info.update(static_cast<Actor&>(*this),delta_time);
   
}


// �`��
void Player::draw() const {
    info.draw();
	glPushMatrix();
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
        info.hit();
    }
}