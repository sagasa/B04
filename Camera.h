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
    //�����Ă��邩�ǂ����H
    bool is_viewing(const BoundingSphere& sphere);
    

private:
    Actor* player_;
};