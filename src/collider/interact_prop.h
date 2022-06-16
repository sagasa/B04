#pragma once

#include <typeinfo>
#include "Actor.h"

//�A�N�^�[�v���p�e�B�N���X
class interact_prop {
public:
	//�R���X�g���N�^
	interact_prop() = default;

	//���z�f�X�g���N�^
	virtual ~interact_prop() = default;

	//�C���^���N�g���󂯂�
	virtual bool can_interact(const Actor& from) = 0;

	static bool try_interact(Actor& victim, const Actor& from)
	{
		try
		{
			auto& actor = dynamic_cast<interact_prop&>(victim);
			//���炷����
			return actor.can_interact(from);
		}
		catch (std::bad_cast&)
		{
			return false;
		}
	}
};