#pragma once
#include "Actor.h"


// �J�����N���X
class Camera : public Actor {
public:
    // �R���X�g���N�^
    Camera(IWorld* world);
    //�X�V
    virtual void update(float delta_time)override;
    // �`��
    virtual void draw() const override;

private:
    //�����Ă��邩�ǂ����H
    bool is_viewing(const BoundingSphere& sphere);
    

private:
    //�v���C���[
    Actor* player_;
    //�Œ�ɂ��邩�ǂ����H
    bool is_fixed_;

};