#include "Field.h"

// �t�B�[���h�̃T�C�Y
const float FieldSize{ 150.0f };

// �R���X�g���N�^
Field::Field(GSuint octree, GSuint collider, GSuint skybox) 
    : octree_{ octree }, collider_{ collider }, skybox_{skybox} 
{
}

// �X�V
void Field::update(float delta_time) {
}

// �`��
void Field::draw() const {
    //gsDrawSkyBox(skybox_);
    gsDrawOctree(octree_);    
}

bool Field::collide(const Line& line, GSvector3* intersect, GSplane* plane)const {
    return gsOctreeCollisionLine(gsGetOctree(octree_), &line.start, &line.end, intersect, plane);
}
bool Field::collide(const Ray& ray, float max_distance, GSvector3* intersect, GSplane* plane)const {
    Line line{ ray.position,ray.position + (ray.direction.normalized() * max_distance) };
    return collide(line, intersect, plane);
}
bool Field::collide(const BoundingSphere& sphere, GSvector3* center)const {
    return gsOctreeCollisionSphere(gsGetOctree(0), &sphere.center, sphere.radius, center);
}