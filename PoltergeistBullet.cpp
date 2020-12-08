#include"PoltergeistBullet.h"
#include"IWorld.h"
#include"Camera.h"

//���S����܂ł̎���
const float Delay_time{3.0f};

PoltergeistBullet::PoltergeistBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity) :
	died_timer_{0.0f} {
	world_ = world;
	name_ = "PoltergeistBullet";
	tag_ = "EnemyAttackTag";
	collider_ = BoundingSphere{0.4f };
	transform_.position(position);
	velocity_ = velocity;
}

//�X�V
void PoltergeistBullet::update(float delta_time) {
	//3�b�������玀�S
	/*if (died_timer_ / 60 >= Delay_time) {
		die();
		return;
	}
	else died_timer_ += delta_time;*/
	//��ʊO�ɏo���玀�S
	if (is_out_camera()) {
		die();
		return;
	}
	//��]����
	transform_.rotate(0.0f,1.0f,1.0f);
	//�ړ�����
	transform_.translate(velocity_, GStransform::Space::World);
}

//�`��
void PoltergeistBullet::draw() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	gsDrawMesh(6);
	glPopMatrix();
	collider().draw();
}

//�Փ˃��A�N�V����
void PoltergeistBullet::react(Actor& other) {
	if (other.tag() == "PlayerTag") {
		die();
	}
}

bool PoltergeistBullet::is_out_camera() const {
	Camera* camera = world_->camera();
	if (camera == nullptr) return false;
	//��ʓ��ɂ�����ړ�����
	GSvector3 to_target = transform_.position() - camera->transform().position();
	//�J�����̑O�x�N�g��
	GSvector3 forward = camera->transform().forward();
	float angle = abs(GSvector3::signed_angle(forward, to_target));
	return (angle >= 45.0f);
}