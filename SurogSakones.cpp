#include "SurogSakones.h"
#include"IWorld.h"
#include"Field.h"
#include"Player.h"

enum {
	MotionIdol1,
	MotionIdol2,
	MotionMove,
	MotionAttack1,
	MotionAttack2,
	MotionDamage,
	MotionDie
};

SurogSakones::SurogSakones(IWorld* world, const GSvector3& position) :
	mesh_{ 12, 12, 12, MotionIdol1 }
{
	world_ = world;
	tag_ = "EnemyTag";
	name_ = "SurogSakones";
	transform_.position(position);
	collider_ = BoundingSphere(5);
	state_ = State::Normal;
	hp_ = 100.0f;
	transform_.rotation(GSquaternion::euler(GSvector3{ 0.0f,-90.0f,0.0f }));

	mesh_.transform(transform_.localToWorldMatrix());

	move_pos_.push_back(transform().position());
	move_pos_.push_back(transform().position() - GSvector3{ 50.0f,0.0f,0.0f });

	destination_ = move_pos_[0];
}
void SurogSakones::update(float delta_time) {
	if (gsGetKeyTrigger(GKEY_UPARROW)) {
		//motion_++;
		flip = true;
	}
	if (gsGetKeyTrigger(GKEY_DOWNARROW)) {
		//motion_--;
		flip = false;
	}
	mesh_.change_motion(motion_);
	mesh_.update(delta_time);
	mesh_.transform(transform_.localToWorldMatrix());

	if (hp_ <= 0)die();

	switch (state_) {
	case State::Apper:appear_update(delta_time); break;
	case State::Normal:normal_update(delta_time); break;
	case State::Turn:turn_update(delta_time); break;
	}
	if (gsGetKeyState(GKEY_RCONTROL)) {
		destination_ = move_pos_[0];
	}
	if (gsGetKeyState(GKEY_LCONTROL)) {
		destination_ = move_pos_[1];
	}
	//turn(delta_time, GSvector3{ 0.0f,180.0f,0.0f });
}
void SurogSakones::late_update(float delta_time) {
	current_flip_ = flip;
}

void SurogSakones::appear_update(float delta_time) {

}
void SurogSakones::normal_update(float delta_time) {
	move(delta_time);
	if (current_flip_ != flip) {
		state_ = State::Turn;
	}
}
void SurogSakones::angry_update(float delta_time) {

}
void SurogSakones::stun_update(float delta_time) {

}
void SurogSakones::die_update(float delta_time) {


}
void SurogSakones::turn_update(float delta_time) {
	turn(delta_time, 0.0f, flip);
	if (mesh_.motion_end_time() <= 0.0f) {
		state_ = State::Normal;
	}
}
void SurogSakones::change_state(State state, GSuint motion) {
	motion_ = motion;
	state_ = state;
	state_timer_ = 0.0f;
}
void SurogSakones::draw()const {
	mesh_.draw();
	//collider().draw();
	debug_draw();
}

void SurogSakones::debug_draw()const {
	// �f�o�b�O�\��
	gsFontParameter(GS_FONT_BOLD, 16, "�l�r �S�V�b�N");
	gsTextPos(0.0f, 0.0f);
	gsDrawText("SurogSakones:���W(%f,%f,%f)", transform_.position().x, transform_.position().y, transform_.position().z);
	gsTextPos(0.0f, 20.0f);
	gsDrawText("SurogSakones:���[�e�[�V����(%f,%f,%f)", transform_.rotation().x, transform_.rotation().y, transform_.rotation().z);
	gsTextPos(0.0f, 40.0f);
	gsDrawText("SurogSakones:�X�P�[��(%f,%f,%f)", transform_.lossyScale().x, transform_.lossyScale().y, transform_.lossyScale().z);
	gsTextPos(0.0f, 60.0f);
	gsDrawText("SurogSakones:���[�V�����ԍ�(%d)", motion_);
	gsTextPos(0.0f, 80.0f);
	gsDrawText("��ԁF(%d)", state_);
}

/**
* PurPose �U����H�炤����
* ���Ǘ\��
*/
void SurogSakones::Damage() {
	hp_ -= 10;
}

void SurogSakones::pshychokinesis(const GSvector3& position) {
	Actor* player = world_->find_actor("Player");
	//�v���C���[���߈˒�������ׁA�߈˒�����Ȃ�������return���鏈������X�ǉ�
	//�v���C���[���k���łȂ���
	if (player != nullptr) {
		//�v���C���[�̃x�N�g�������߂�
		GSvector3 to_player = (player->transform().position() - transform().position()).normalized();
		//�����o������
	}
}
void SurogSakones::move(float delta_time) {
	if (GSvector3::distance(transform_.position(), destination_) <= 0.5f) {
		transform_.position(destination_);
	}
	else if (transform_.position() < destination_) {
		velocity_ = GSvector3{ 1.0f,0.0f,0.0f };
		transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	}
	else if (transform_.position() > destination_) {
		velocity_ = GSvector3{ -1.0f,0.0f,0.0f };
		transform_.translate(velocity_ * delta_time, GStransform::Space::World);
	}
}

void SurogSakones::move_attack(float delta_time) {

}

void SurogSakones::turn(float delta_time, float slow_value, bool flip) {
	if (flip)to_rotate_ = GSvector3{ 0.0f, 90.0f ,0.0f };
	else to_rotate_ = { 0.0f, -90.0f ,0.0f };
	motion_ = MotionAttack1;
	if (GSquaternion::angle(transform_.rotation(), GSquaternion::euler(to_rotate_)) >= 5.0f) {
		transform_.rotate(GSvector3{ 0.0f,1.0f,0.0f }, 5.0f / delta_time, GStransform::Space::World);
	}
	else {
		transform_.rotation(GSquaternion::euler(to_rotate_));
		state_ = State::Normal;		
	}
}