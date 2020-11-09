#include "TestObj.h"

#include <iostream>

void TestObj::change()
{
    velocity_.x = 1- rand() / 32767.0f * 2;
    velocity_.y = 1-rand() / 32767.0f * 2;
}

TestObj::TestObj(IWorld* world, const GSvector3& position,int id)
{
    world_ = world;
    transform_.position(position);
    tag_ = "obj " + std::to_string(id);
    change();
    color = GSvector3{ rand() / 32767.0f ,rand() / 32767.0f ,rand() / 32767.0f };
}
const float MovingRangeX = 50;
const float MovingRangeY = 50;

void TestObj::draw() const
{
    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);

    const GSvector3 vec = transform_.position();

    glMultMatrixf(transform_.localToWorldMatrix());

    if (isHit)
        glColor4f(color.r, color.g,color.b,0.5f);
    else 
        glColor3f(color.r, color.g, color.b);
    glBegin(GL_QUADS);
    glVertex2f(1, 1);
    glVertex2f(-1, 1);
    glVertex2f(-1, -1);
    glVertex2f(1, -1);
    glEnd();

    glColor3f(1, 1, 1);
    glPopAttrib();
    glPopMatrix();
}

const GSvector3 gravity{ 0.0f, 0.1f, 0.0f };
#define	INRANGE( v, l, h )	(l<=v && v<= h)
void TestObj::update(float delta_time)
{
    last_transform_ = transform_;
    isHit = false;

    velocity_ -= gravity * delta_time;
	
    // ���W���擾
    GSvector3 position = transform_.position();
    // ���W���X�V
    position += velocity_*delta_time;

    //std::cout << (INRANGE(position.x, -MovingRangeX, MovingRangeX)) << std::endl;
	
    // ��ʊO�ɏo�Ȃ��悤�Ɉړ��͈͂𐧌�����
    if (!INRANGE(position.x, -MovingRangeX, MovingRangeX) ||
        !INRANGE(position.y, -MovingRangeY, MovingRangeY))
        change();
	
    position.x = CLAMP(position.x, -MovingRangeX, MovingRangeX);
    position.y = CLAMP(position.y, -MovingRangeY, MovingRangeY);
    // ���W�̐ݒ�

    transform_.position(position);
}
