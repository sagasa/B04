#ifndef ACTOR_PROP_H_
#define ACTOR_PROP_H_
#include <typeinfo>


//�A�N�^�[�v���p�e�B�N���X
class DamageProp {

public:
	//�R���X�g���N�^
	DamageProp() = default;

	//���z�f�X�g���N�^
	virtual ~DamageProp() = default;

	//�̗͂�ݒ�
	void set_hp(float hp) {
		hp_ = hp;
	}

	//�̗͂��擾
	float hp() const {
		return hp_;
	}


	//�U�����󂯂�
	virtual bool on_hit(const Actor& attacker, float atk_power) = 0;

	static bool do_attack(Actor& victim,const Actor& attacker, float value = 1)
	{
		try
		{
			auto& actor = dynamic_cast<DamageProp&>(victim);
			//���炷����
			return actor.on_hit(attacker, value);
		}catch (std::bad_cast&)
		{
			return false;
		}
	}

protected:
	//�̗�
	float hp_{ 0.0f };
};
#endif

