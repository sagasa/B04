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
	float hp() const {
		return hp_;
	}

	//�U���͂�ݒ�
	void set_atk_power(float atk) {
		atk_power_ = atk;
	}

	//�U���͂��擾
    float atk_power()const {
		return atk_power_;
	}

	//�U�����󂯂�
	void hit(float atk_power);

protected:
	//�̗�
	float hp_{ 0.0f };
	//�U����
	float atk_power_{ 0.0f };

};



#endif

