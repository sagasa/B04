#ifndef SUROG_SAKONES
#define SUROG_SAKONES

class IWorld;

enum State {
    Unkown,
    Apper,
    Normal,
    Angry
};

#include "Actor.h"
class SurogSakones :
    public Actor
{
public:
    SurogSakones(IWorld* world, const GSvector3& position);
    virtual void update(float delta_time)override;
    virtual void draw()const override;

private:    
    //”O“®UŒ‚
    void PsychokinesisAttack(const GSvector3& position,const GSvector3& velocity);
    //•’Ê‚ÌˆÚ“®(”½‘Î‘¤‚ÉˆÚ“®‚·‚é)
    void Move(bool flag);
    //ˆÚ“®‚µ‚Â‚ÂUŒ‚
    void MoveAttack();

private:
    State state_;
    float hp_{ 0.0f };

};

#endif