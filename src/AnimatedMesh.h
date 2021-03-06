#ifndef ANIMATED_MESH_H_
#define ANIMATED_MESH_H_

#include<gslib.h>
#include<array>
#include "Assets.h"

class AnimatedMesh
{
public:
	AnimatedMesh(Mesh mesh, Skeleton skeleton, Animation animation, GSuint motion = 0,bool loop = true);
	void update(float delta_time);
	void draw()const;
	void draw_ex() const;
	bool change_motion(GSuint motion,bool loop = true);
	void transform(const GSmatrix4& matrix);
	float motion_end_time()const;
	const GSmatrix4& bone_matrices(int bone_no)const;
private:
	//???b?V??
	GSuint mesh_;
	GSuint skeleton_;
	GSuint animation_;
	GSuint motion_;
	GSfloat motion_timer_;
	bool motion_loop_{ true };
	GSmatrix4 transform_;	

	GSuint prev_motion_;
	GSfloat prev_motion_timer_;
	GSfloat lerp_timer_;
	std::array<GSmatrix4, 256> local_bone_matrices_;
	std::array<GSmatrix4, 256> world_bone_matrices_;
};

#endif