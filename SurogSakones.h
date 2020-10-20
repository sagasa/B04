#ifndef SUROG_SAKONES
#define SUROG_SAKONES

class IWorld;

#include "Actor.h"
class SurogSakones :
    public Actor
{
public:
    enum class State {
        Unkown,
        Apper,
        Normal,
        Angry
    };
public:
    SurogSakones(IWorld* world, const GSvector3& position);
    virtual void update(float delta_time)override;
    virtual void draw()const override;
    void Damage();

private:    
    //�O���U��
    void pshychokinesis(const GSvector3& position);
    //���ʂ̈ړ�(���Α��Ɉړ�����)
    void move();
    //�ړ����U��
    void move_attack();

private:
    State state_;
    float hp_{ 0.0f };

};

#endif