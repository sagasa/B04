#include "HPBar.h"

#include <iostream>


#include "Assets.h"


void HPBar::set_HP(const int hp)
{
	anim_count_ = max_anim_count_;
    current_ = hp;
}

void HPBar::set_HP_NoAnim(const int hp)
{
    current_ = hp;
	//アニメーション中でないなら
    if (0 < anim_count_)
		prev_ = hp;
}

void HPBar::update(const float delta)
{
	if (0 < anim_count_)
	{
		anim_count_ -= delta;
		if( anim_count_<0)
		{
            anim_count_ = 0;
            prev_ = current_;
		}
	}
}

void HPBar::draw(const GSvector2& hp_position) const
{
    float main_width = width_ * current_ / max_;
	//増加時
    if (prev_ < current_ && 0 < anim_count_)
        main_width*= 1-anim_count_ / max_anim_count_;
	
    //HPゲージ
    GSrect mainRect{ 0.0f,0.0f, main_width,50.0f };
    static const GSvector2 scale{ 1.0f,0.5f };
    gsDrawSprite2D(Texture_BossHP, &hp_position, &mainRect, NULL, &color_, &scale, 0.0f);

	//減少時のみ
	if(current_<prev_&&0<anim_count_)
	{
        std::cout << "anim " << anim_count_ << "\n";
        float sub_width = anim_count_/max_anim_count_*width_;
        GSrect subRect{ main_width,0.0f,main_width + sub_width,50.0f };
        gsDrawSprite2D(Texture_BossHP, &hp_position, &subRect, NULL, &color_sub_, &scale, 0.0f);
	}

    //上下の蓋
    GSrect frame_rect{ 0.0f,0.0f,50.0f,52.0f };
    float mag_scale = width_ / 50.0f;
    GSvector2 frame_scale{ mag_scale,scale.y };
    gsDrawSprite2D(Texture_BossHPFrame, &hp_position, &frame_rect, NULL, NULL, &frame_scale, 0.0f);

    //左右の蓋描画
    float interval = 5.0f * scale.y;
    GSvector2 framelid_left_position{ hp_position.x - interval,hp_position.y + interval };
    GSvector2 framelid_right_position{ hp_position.x + width_,hp_position.y + interval };
    GSrect frame_lid_rect{ 0.0f,0.0f,5.0f,42.0f };
    GSvector2 frame_lid_scale{ scale.y,scale.y };
    gsDrawSprite2D(Texture_BossHPFrame_Lid, &framelid_left_position, &frame_lid_rect, NULL, NULL, &frame_lid_scale, 0.0f);
    gsDrawSprite2D(Texture_BossHPFrame_Lid, &framelid_right_position, &frame_lid_rect, NULL, NULL, &frame_lid_scale, 0.0f);
}

