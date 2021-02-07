#include "DeathSmoke.h"
#include "ParticleManager.h"
#include "IWorld.h"
#include "ParticleManager.h"

//�����蔻��̈ʒu�̍���
const float Height{ 0.75f };
//�����蔻��̔��a
const float Radius{ 1.0f };
//�G�t�F�N�g�Ŕ��a
const float Efect_radius{ 1.2f };
//�Z���^�[
const GSvector3 Center{ 0,Height,0 };
//�����̃I�t�Z�b�g
const float FootOffset{ 0.1f };
//�G�t�F�N�g�C���^�[�o��
const float Interval{ 240.0f };



DeathSmoke::DeathSmoke(IWorld* world, const GSvector3& position) 
{
	//���[���h
	world_ = world;
	//���O�̐ݒ�
	name_ = "DeathSmoke";
	//�^�O���̐ݒ�
	tag_ = "FieldDamage";
	//�̗͖���
	//�Փ˔��苅�̐ݒ�
	collider_ = BoundingSphere{ Radius,Center };
	transform_.position(position);
	//mesh_.transform(transform_.localToWorldMatrix());
}
void DeathSmoke::update(float delta_time)
{
	GSvector3 position{ transform_.position() };
	GSvector3 center_position{position};//�^�񒆂̃G�t�F�N�g�̈ʒu
	//�^�񒆂ɒ���
	center_position.y= position.y  + Height;
	GSvector3 top_position{ center_position };//��
	top_position.y +=Radius/2;
	GSvector3 under_position{ center_position };//��
	under_position.y -= Radius/2;
	GSvector3 right_position{ center_position };//�E
	right_position.x += Radius / 2;
	GSvector3 left_position{ center_position };//��
	left_position.x -= Radius / 2;

	world_->particle_manager()->death_smoke2(center_position);
	/*world_->particle_manager()->death_smoke(top_position);
	world_->particle_manager()->death_smoke(under_position);
	world_->particle_manager()->death_smoke(right_position);
	world_->particle_manager()->death_smoke(left_position);*/

}
void DeathSmoke::draw() const
{
	collider().draw();
}
void DeathSmoke::react(Actor& other)
{
	if (other.tag() == "PlayerTag")
	{
		DamageProp::do_attack(other, *this, 100);
	}
}