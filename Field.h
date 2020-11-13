#ifndef FIELD_H_
#define FIELD_H_

#include <gslib.h>
#include"Line.h"
#include"Ray.h"
#include"BoundingSphere.h"

// �t�B�[���h�N���X
class Field {
public:
	Field(GSuint octree, GSuint collider, GSuint skybox);

	void update(float delta_time);
	void draw()const;

	bool collide(const Line& line, GSvector3* intersect = nullptr, GSplane* plane = nullptr)const;
	bool collide(const Ray& ray, float max_distance, GSvector3* intersect = nullptr, GSplane* plane = nullptr)const;
	bool collide(const BoundingSphere& sphere, GSvector3* center = nullptr)const;

	Field(const Field& other) = delete;
	Field& operator = (const Field& other) = delete;

private:
	GSuint octree_;
	GSuint collider_;
	GSuint skybox_;
};

#endif