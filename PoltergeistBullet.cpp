#include"PoltergeistBullet.h"
#include"IWorld.h"
#include"Camera.h"
#include"Assets.h"
#include"Field.h"

//���S����܂ł̎���
const float Delay_time{3.0f};

PoltergeistBullet::PoltergeistBullet(IWorld* world, const GSvector3& position, const GSvector3& velocity, float atk_power) :
	died_timer_{0.0f} {
	//���[���h�̐ݒ�
	world_ = world;
	//���O�̐ݒ�
	name_ = "PoltergeistBullet";
	//�^�O���̐ݒ�
	tag_ = "EnemyAttack";
	//�Փ˔��苅�̐ݒ�
	collider_ = BoundingSphere{0.4f};
	//���W�̏�����
	transform_.position(position);
	//
	velocity_ = velocity;
	//ActorProp���p�����Ă��邩�H
	hit_ = true;
	//�U���͂̐ݒ�
	atk_power_ = atk_power;
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
	//�ǂƂ̏Փ˔���
	collide_field();
}

//�`��
void PoltergeistBullet::draw() const {
	glPushMatrix();
	glMultMatrixf(transform_.localToWorldMatrix());
	gsDrawMesh(Mesh_Book);
	glPopMatrix();
	collider().draw();
}

//�Փ˃��A�N�V����
void PoltergeistBullet::react(Actor& other) {
	//�G�l�~�[�ȊO�ɓ��������玀�S
	if (other.tag() != "EnemyTag") {
		DamageProp::do_attack(other, *this, atk_power_);
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

//�ǂƂ̏Փ˔���
void PoltergeistBullet::collide_field() {
	//�ǂƂ̏Փ˔���(���̂Ƃ̔���)
	GSvector3 center;//�Փˌ�̋��̂̒��S���W
	if (world_->field()->collide(collider(),&center)) {
		die();
	}
}