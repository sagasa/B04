#include"AttackCollider.h"
#include"DamageProp.h"
#include"World.h"


//�R���X�g���N�^
AttackCollider::AttackCollider(IWorld* world, const BoundingSphere& collider,
	const std::string& tag, const std::string& name, std::string& user_name, float lifespan, float delay, float atk_power):
	lifespan_timer_{ lifespan }, delay_timer_{ delay }, atk_power_{ atk_power }, before_position_{GSvector3::zero()}
{
	//���[���h�̐ݒ�
	world_ = world;
	//�^�O���̐ݒ�
	tag_ = tag;
	//���O�̐ݒ�
	name_ = name;
	//�Փ˔����������
	collider_ = BoundingSphere{ collider.radius };
	//���W�̏�����
	transform_.position(collider.center);
	//���G��Ԃɂ���
	enable_collider_ = false;

	user_ = user_name;
}

//�X�V
void AttackCollider::update(float delta_time)
{
	//�x�����Ԃ��o�߂�����Փ˔����L���ɂ���
	if (delay_timer_ <= 0.0f) {
		//�Փ˔����L���ɂ���
		enable_collider_ = true;
		//�������s�����玀�S
		if (lifespan_timer_ <= 0.0f) {
			die();
		}
		//�����̍X�V
		lifespan_timer_ -= delta_time;
	}
	//�x�����Ԃ̍X�V
	delay_timer_ -= delta_time;
	Actor* user = world_->find_actor(user_);
	if (user == nullptr) return;
	GSvector3 position = (user->transform().position() - before_position_);
	before_position_ = user->transform().position();
	transform_.position() += position;
}

//�`��
void AttackCollider::draw() const
{
	collider().draw();
}

//�Փ˃��A�N�V����
void AttackCollider::react(Actor& other)
{
	if(DamageProp::do_attack(other,*this,atk_power_))
		//�Փ˂����玀�S
		die();
}