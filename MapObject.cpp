#include "MapObject.h"

MapObject::MapObject(IWorld* world, const GSvector3& position)
{
    world_ = world;
    transform_.position(position);
	using namespace collisions;
	mat.is_static = true;
	box2d = Box2D{ Vec2{0,0},Vec2{40,30},mat };
}

void MapObject::draw() const
{
    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);

    const GSvector3 vec = transform_.position();
    float x = box2d.size.x / 2, y = box2d.size.y / 2;
    glMultMatrixf(transform_.localToWorldMatrix());
    glColor3f(1, 1, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(-x, y);
    glVertex2f(-x, -y);
    glVertex2f(x, -y);
    glEnd();

    glColor3f(1, 1, 1);
    glPopAttrib();
    glPopMatrix();
}

