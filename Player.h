#pragma once
#include "Actor.h"
#include "DamageProp.h"
#include "AnimatedMesh.h"

// �v���[��
class Player : public Actor, public DamageProp {
    

protected:
    int jump_count_;
    float jump_force_;
	
    bool on_ground_;
    //�A�j���[�V�������b�V��
    AnimatedMesh mesh_;
  
	//���肪�d����
    bool is_soft_;

	//�㉺�̍����g��
    float height_ext_ = 0;
	
    //���x��K�� �t�B�[���h�Ƃ̏Փˏ���
    void update_physics(const float delta_time);
	
    //�t�B�[���h�Ƃ̏Փˏ���
    void jump(const int count,const float force);

    //���͂�vec2�Ƃ��Ď擾
    GSvector2 static get_input();
public:	
    // �R���X�g���N�^
    Player(IWorld* world, const GSvector3& position,const AnimatedMesh& mesh);

    virtual void wake_up(const int base_hp) = 0;

    virtual void stop() = 0;
};
