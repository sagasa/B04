#pragma once
#include "GSgame.h"

class HPBar
{
	int max_;
	int current_{0};
	int prev_{ 0 };
	GScolor color_;
	GScolor color_sub_;
	int width_{80};
	
	
	float anim_count_{0};
	const float max_anim_count_{ 40 };

public:
	HPBar(const int max, const int width,const GScolor& color, const GScolor& color_sub)
	{
		max_ = max;
		width_ = width;
		color_ = color;
		color_sub_ = color_sub;
	}

	void set_HP(const int hp);
	
	void set_HP_NoAnim(const int hp);
	void update(const float delta);
	void draw(const GSvector2& pos) const;
};

