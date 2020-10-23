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

		// �����̒����A�����̑��̍��W�y��t���Z�o
		float len = calcPointLineDist2D(p, seg, h, t);

		if (isSharpAngle(p, seg.p, e) == false) {
			// �n�_���̊O��
			h = seg.p;
			return (seg.p - p).length();
		}
		else if (isSharpAngle(p, e, seg.p) == false) {
			// �I�_���̊O��
			h = e;
			return (e - p).length();
		}

		return len;
	}

	float calcLineLineDist2D(const Line2D& l1, const Line2D& l2, Vec2& p1, Vec2& p2, float& t1, float& t2)
	{

		// 2���������s�H
		if (isParallel(l1.v, l2.v) == true) {

			// �_P11�ƒ���L2�̍ŒZ�����̖��ɋA��
			float len = calcPointLineDist2D(l1.p, l2, p2, t2);
			p1 = l1.p;
			t1 = 0.0f;

			return len;
		}

		// 2�����͂˂���֌W
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

        // S1���k�ނ��Ă���H
        if (s1.v.lengthSq() < _OX_EPSILON_) {
            // S2���k�ށH
            if (s2.v.lengthSq() < _OX_EPSILON_) {
                // �_�Ɠ_�̋����̖��ɋA��
                float len = (s2.p - s1.p).length();
                p1 = s1.p;
                p2 = s2.p;
                t1 = t2 = 0.0f;
                return len;
            }
            else {
                // S1�̎n�_��S2�̍ŒZ���ɋA��
                float len = calcPointSegmentDist2D(s1.p, s2, p2, t2);
                p1 = s1.p;
                t1 = 0.0f;
                clamp01(t2);
                return len;
            }
        }

        // S2���k�ނ��Ă���H
        else if (s2.v.lengthSq() < _OX_EPSILON_) {
            // S2�̎n�_��S1�̍ŒZ���ɋA��
            float len = calcPointSegmentDist2D(s2.p, s1, p1, t1);
            p2 = s2.p;
            clamp01(t1);
            t2 = 0.0f;
            return len;
        }

        /* �������m */

        // 2���������s�������琂���̒[�_�̈��P1�ɉ�����
        if (isParallel(s2.v, s1.v) == true) {
            t1 = 0.0f;
            p1 = s1.p;
            float len = calcPointSegmentDist2D(s1.p, s2, p2, t2);
            if (0.0f <= t2 && t2 <= 1.0f)
                return len;
        }
        else {
            // �����͂˂���̊֌W
            // 2�����Ԃ̍ŒZ���������߂ĉ���t1,t2�����߂�
            float len = calcLineLineDist2D(s1, s2, p1, p2, t1, t2);
            if (
                0.0f <= t1 && t1 <= 1.0f &&
                0.0f <= t2 && t2 <= 1.0f
                ) {
                return len;
            }
        }

        // �����̑����O�ɂ��鎖������
        // S1����t1��0�`1�̊ԂɃN�����v���Đ������~�낷
        clamp01(t1);
        p1 = s1.getPoint(t1);
        float len = calcPointSegmentDist2D(p1, s2, p2, t2);
        if (0.0f <= t2 && t2 <= 1.0f)
            return len;

        // S2�����O�������̂�S2�����N�����v�AS1�ɐ������~�낷
        clamp01(t2);
        p2 = s2.getPoint(t2);
        len = calcPointSegmentDist2D(p2, s1, p1, t1);
        if (0.0f <= t1 && t1 <= 1.0f)
            return len;

        // �o���̒[�_���ŒZ�Ɣ���
        clamp01(t1);
        p1 = s1.getPoint(t1);
        return (p2 - p1).length();
    }

}

