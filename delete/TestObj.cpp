#include "TestObj.h"

#include <iostream>


void TestObj::change()
{
    velocity_.x = 1- rand() / 32767.0f * 2;
    velocity_.y = 1-rand() / 32767.0f * 2;
}

TestObj::TestObj(IWorld* world, const GSvector3& position, const GSvector2& vel,int id)
{
    world_ = world;
    transform_.position(position);
    tag_ = "obj " + std::to_string(id);
    velocity_ = vel;
   // change();
    color = GSvector3{ rand() / 32767.0f ,rand() / 32767.0f ,rand() / 32767.0f };
    using namespace collisions;
    box2d = Box2D{ Vec2{0,0},Vec2{2,2},mat };
}
const float MovingRangeX = 80;
const float MovingRangeY = 80;

void TestObj::draw() const
{
    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);

    const GSvector3 vec = transform_.position();

    glMultMatrixf(transform_.localToWorldMatrix());

    if (isHit)
        glColor4f(color.r, color.g,color.b,0.2f);
    else 
        glColor3f(color.r, color.g, color.b);
    glBegin(GL_QUADS);
    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glEnd();
	
   // gsTextDraw(GS_FONT_BOLD, 16, "MS �S�V�b�N", format("Name %s", tag_));

    glColor3f(1, 1, 1);
    glPopAttrib();
    glPopMatrix();
}



const GSvector3 gravity{ 0.0f, 0.1f, 0.0f };
#define	INRANGE( v, l, h )	(l<=v && v<= h)
void TestObj::update(float delta_time)
{
    isHit = false;

    //velocity_ -= gravity * delta_time;
	
    // ���W���擾
    GSvector3 position = transform_.position();
    // ���W���X�V
    position += velocity_*delta_time;

    //std::cout << (INRANGE(position.x, -MovingRangeX, MovingRangeX)) << std::endl;
	
    // ��ʊO�ɏo�Ȃ��悤�Ɉړ��͈͂𐧌�����
    if (!INRANGE(position.x, -MovingRangeX, MovingRangeX) ||
        !INRANGE(position.y, -MovingRangeY, MovingRangeY))
    {
        //change();
        die();
        //position.y = 10;
        //velocity_.y = 0;
    }
        
    position.x = CLAMP(position.x, -MovingRangeX, MovingRangeX);
    position.y = CLAMP(position.y, -MovingRangeY, MovingRangeY);
    // ���W�̐ݒ�

    transform_.position(position);
}
