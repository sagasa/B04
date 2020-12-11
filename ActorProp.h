#ifndef ACTOR_PROP_H_
#define ACTOR_PROP_H_



//�A�N�^�[�v���p�e�B�N���X
class ActorProp {
public:
	//�R���X�g���N�^
	ActorProp() = default;

	//���z�f�X�g���N�^
	virtual ~ActorProp() = default;

	//�̗͂�ݒ�
	void set_hp(float hp) {
		hp_ = hp;
	}

	//�̗͂��擾
	const float hp() const {
		return hp_;
	}

	//�U���͂�ݒ�
	void set_atk_power(float atk) {
		atk_power_ = atk;
	}

	//�U���͂��擾
	const float atk_power()const {
		return atk_power_;
	}

protected:
	//�̗�
	float hp_{ 0.0f };
	//�U����
	float atk_power_{ 0.0f };

};



#endif

