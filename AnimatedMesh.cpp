#include "AnimatedMesh.h"

const GSfloat LerpTime{ 10.0f };

AnimatedMesh::AnimatedMesh(GSuint mesh, GSuint skeleton, GSuint animation, GSuint motion) :
	mesh_{ mesh },
	skeleton_{ skeleton },
	animation_{ animation },
	motion_{ motion },
	transform_{ GS_MATRIX4_IDENTITY }{
}


void AnimatedMesh::update(float delta_time) {
	motion_timer_ += delta_time;
	lerp_timer_ = MIN(lerp_timer_ + delta_time, LerpTime);
}

void AnimatedMesh::draw()const {
	gsDisable(GS_CALC_SKELETON);
	gsBindSkeleton(skeleton_);
	gsSetMatrixSkeleton(world_bone_matrices_.data());
	gsDrawMesh(mesh_);
	gsEnable(GS_CALC_SKELETON);
}
void AnimatedMesh::change_motion(GSuint motion) {
	if (motion_ == motion)return;
	prev_motion_ = motion_;
	prev_motion_timer_ = motion_timer_;
	lerp_timer_ = 0.0f;
	motion_ = motion;
	motion_timer_ = 0.0f;
}
void AnimatedMesh::transform(const GSmatrix4& matrix) {
	gsCalculateAnimationLerp(
		animation_, prev_motion_, prev_motion_timer_,
		animation_, motion_, motion_timer_,
		lerp_timer_ / LerpTime,
		local_bone_matrices_.data()
	);
	gsBindSkeleton(skeleton_);
	gsCalculateSkeleton(&matrix, local_bone_matrices_.data(), world_bone_matrices_.data());
}
float AnimatedMesh::motion_end_time()const {
	return gsGetEndAnimationTime(animation_, motion_);
}
const GSmatrix4& AnimatedMesh::bone_matrices(int bone_no)const {
	return world_bone_matrices_[bone_no];
}