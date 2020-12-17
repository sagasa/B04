#pragma once
#include "Actor.h"
#include "ActorProp.h"
#include "AnimatedMesh.h"

// �v���[��
class Player : public Actor, public ActorProp {
    int jump_count_;
    float jump_force_;

protected:
    bool on_ground_;
    //�A�j���[�V�������b�V��
    AnimatedMesh mesh_;
    //���[�V�����ԍ�
    GSuint motion_;
    //���[�V�����̃��[�v�w��
    bool motion_loop_;
	//���肪�d����
    bool is_soft_;
	
    //���x��K�� �t�B�[���h�Ƃ̏Փˏ���
    void update_physics(const float delta_time);
	
    //�t�B�[���h�Ƃ̏Փˏ���
    void jump(const int count,const float force);

    //���͂�vec2�Ƃ��Ď擾
    GSvector2 static get_input();
public:	
    // �R���X�g���N�^
    Player(IWorld* world, const GSvector3& position,const AnimatedMesh& mesh);
    // �`��
    void draw() const override;
};
