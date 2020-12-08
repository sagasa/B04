#include"PoltergeistBullet.h"

//���S����܂ł̎���
const float Delay_time{3.0f};

PoltergeistBullet::PoltergeistBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity) :
	died_timer_{0.0f} {
	world_ = world;
	name_ = "PoltergeistBullet";
	tag_ = "EnemyTag";
	collider_ = BoundingSphere{0.25f };
	transform_.position(position);
	velocity_ = velocity;
}

//�X�V
void PoltergeistBullet::update(float delta_time) {
	//3�b��������
	if (died_timer_ / 60 >= Delay_time) {
		die();
		return;
	}
	else died_timer_ += delta_time;
	//�ړ�����
	transform_.translate(velocity_, GStransform::Space::World);
}

//�`��
void PoltergeistBullet::draw() const {
	collider().draw();
}

//�Փ˃��A�N�V����
void PoltergeistBullet::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		die();
	}
}