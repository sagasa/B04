#include "Collision.h"


namespace collisions {
	HitResult::HitResult()
	{
		hit = false;
	}
	HitResult::HitResult(const Vec2& vec)
	{
		pos = vec;
		hit = true;
	}


	float calcPointLineDist2D(const Vec2& p, const Line2D& l, Vec2& h, float& t)
	{
		float lenSqV = l.v.lengthSq();
		t = 0.0f;
		if (lenSqV > 0.0f)
			t = l.v.dot(p - l.p) / lenSqV;
		h = l.p + t * l.v;
		return (h - p).length();
	}

	float calcPointSegmentDist2D(const Vec2& p, const Segment2D& seg, Vec2& h, float& t)
	{

		const Vec2 e = seg.getEndPoint();

		// 垂線の長さ、垂線の足の座標及びtを算出
		float len = calcPointLineDist2D(p, seg, h, t);

		if (isSharpAngle(p, seg.p, e) == false) {
			// 始点側の外側
			h = seg.p;
			return (seg.p - p).length();
		}
		else if (isSharpAngle(p, e, seg.p) == false) {
			// 終点側の外側
			h = e;
			return (e - p).length();
		}

		return len;
	}

	float calcLineLineDist2D(const Line2D& l1, const Line2D& l2, Vec2& p1, Vec2& p2, float& t1, float& t2)
	{

		// 2直線が平行？
		if (isParallel(l1.v, l2.v) == true) {

			// 点P11と直線L2の最短距離の問題に帰着
			float len = calcPointLineDist2D(l1.p, l2, p2, t2);
			p1 = l1.p;
			t1 = 0.0f;

			return len;
		}

		// 2直線はねじれ関係
		float DV1V2 = l1.v.dot(l2.v);
		float DV1V1 = l1.v.lengthSq();
		float DV2V2 = l2.v.lengthSq();
		Vec2 P21P11 = l1.p - l2.p;
		t1 = (DV1V2 * l2.v.dot(P21P11) - DV2V2 * l1.v.dot(P21P11)) / (DV1V1 * DV2V2 - DV1V2 * DV1V2);
		p1 = l1.getPoint(t1);
		t2 = l2.v.dot(p1 - l2.p) / DV2V2;
		p2 = l2.getPoint(t2);

		return (p2 - p1).length();
	}

	float calcSegmentSegmentDist2D(const Segment2D& s1, const Segment2D& s2, Vec2& p1, Vec2& p2, float& t1, float& t2)
    {

        // S1が縮退している？
        if (s1.v.lengthSq() < _OX_EPSILON_) {
            // S2も縮退？
            if (s2.v.lengthSq() < _OX_EPSILON_) {
                // 点と点の距離の問題に帰着
                float len = (s2.p - s1.p).length();
                p1 = s1.p;
                p2 = s2.p;
                t1 = t2 = 0.0f;
                return len;
            }
            else {
                // S1の始点とS2の最短問題に帰着
                float len = calcPointSegmentDist2D(s1.p, s2, p2, t2);
                p1 = s1.p;
                t1 = 0.0f;
                clamp01(t2);
                return len;
            }
        }

        // S2が縮退している？
        else if (s2.v.lengthSq() < _OX_EPSILON_) {
            // S2の始点とS1の最短問題に帰着
            float len = calcPointSegmentDist2D(s2.p, s1, p1, t1);
            p2 = s2.p;
            clamp01(t1);
            t2 = 0.0f;
            return len;
        }

        /* 線分同士 */

        // 2線分が平行だったら垂線の端点の一つをP1に仮決定
        if (isParallel(s2.v, s1.v) == true) {
            t1 = 0.0f;
            p1 = s1.p;
            float len = calcPointSegmentDist2D(s1.p, s2, p2, t2);
            if (0.0f <= t2 && t2 <= 1.0f)
                return len;
        }
        else {
            // 線分はねじれの関係
            // 2直線間の最短距離を求めて仮のt1,t2を求める
            float len = calcLineLineDist2D(s1, s2, p1, p2, t1, t2);
            if (
                0.0f <= t1 && t1 <= 1.0f &&
                0.0f <= t2 && t2 <= 1.0f
                ) {
                return len;
            }
        }

        // 垂線の足が外にある事が判明
        // S1側のt1を0〜1の間にクランプして垂線を降ろす
        clamp01(t1);
        p1 = s1.getPoint(t1);
        float len = calcPointSegmentDist2D(p1, s2, p2, t2);
        if (0.0f <= t2 && t2 <= 1.0f)
            return len;

        // S2側が外だったのでS2側をクランプ、S1に垂線を降ろす
        clamp01(t2);
        p2 = s2.getPoint(t2);
        len = calcPointSegmentDist2D(p2, s1, p1, t1);
        if (0.0f <= t1 && t1 <= 1.0f)
            return len;

        // 双方の端点が最短と判明
        clamp01(t1);
        p1 = s1.getPoint(t1);
        return (p2 - p1).length();
    }
}

