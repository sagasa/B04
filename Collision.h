#pragma once
#include "CollisionType.h"

namespace collisions {

#define _OX_EPSILON_	0.000001f	// 誤差
	struct HitResult
	{
		HitResult();

		HitResult(const Vec2& vec);

		~HitResult() = default;
		
		operator bool() const { return hit; };

	private:
		//これポインタのほうがいいですか？
		Vec2 pos;
		//nullならみたいな処理が書きたかった
		bool hit;
	};

	const HitResult NoHit;
	
	HitResult isHit(const Vec2& vec);

    // 垂直関係にある？
	inline bool isSharpAngle(const Vec2& p1, const Vec2& p2, const Vec2& p3) {
		return (p1 - p2).dot(p3 - p2) >= 0.0f;
	}

    // 点と直線の最短距離（2D版）
    // p : 点
    // l : 直線
    // h : 点から下ろした垂線の足（戻り値）
    // t :ベクトル係数（戻り値）
    // 戻り値: 最短距離
    float calcPointLineDist2D(const Vec2& p, const Line2D& l, Vec2& h, float& t);

    float calcPointSegmentDist2D(const Vec2& p, const Segment2D& seg, Vec2& h, float& t);

    // 0～1の間にクランプ
    inline void clamp01(float& v) {
        if (v < 0.0f)
            v = 0.0f;
        else if (v > 1.0f)
            v = 1.0f;
    }

    inline bool isParallel(Vec2 v1, Vec2 v2) {
            float d = v1.CCW(v2);
            return (-_OX_EPSILON_ < d&& d < _OX_EPSILON_);	// 誤差範囲内なら平行と判定 
    }

    // 2直線の最短距離（2D版）
// l1 : L1
// l2 : L2
// p1 : L1側の垂線の足（戻り値）
// p2 : L2側の垂線の足（戻り値）
// t1 : L1側のベクトル係数（戻り値）
// t2 : L2側のベクトル係数（戻り値）
// 戻り値: 最短距離
    float calcLineLineDist2D(const Line2D& l1, const Line2D& l2, Vec2& p1, Vec2& p2, float& t1, float& t2);

    // 2線分の最短距離（2D版）
// s1 : S1(線分1)
// s2 : S2(線分2)
// p1 : S1側の垂線の足（戻り値）
// p2 : S2側の垂線の足（戻り値）
// t1 : S1側のベクトル係数（戻り値）
// t2 : S2側のベクトル係数（戻り値）
// 戻り値: 最短距離
    float calcSegmentSegmentDist2D(const Segment2D& s1, const Segment2D& s2, Vec2& p1, Vec2& p2, float& t1, float& t2);
}