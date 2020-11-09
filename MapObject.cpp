#include "MapObject.h"

MapObject::MapObject(IWorld* world, const GSvector3& position)
{
    world_ = world;
    transform_.position(position);
	last_transform_ = transform_;
	using namespace collisions;
	mat.is_static = true;
	box2d = Box2D{ Vec2{0,0},Vec2{40,4},mat };
}

void MapObject::draw() const
{
    glPushMatrix();
    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);

    const GSvector3 vec = transform_.position();

    glMultMatrixf(transform_.localToWorldMatrix());
    glScalef(box2d.size.x, box2d.size.y,1);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glVertex2f(0.5, 0.5);
    glVertex2f(-0.5, 0.5);
    glVertex2f(-0.5, -0.5);
    glVertex2f(0.5, -0.5);
    glEnd();

    glColor3f(1, 1, 1);
    glPopAttrib();
    glPopMatrix();
}

