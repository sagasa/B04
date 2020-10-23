#pragma once
#include "CollisionType.h"

namespace collisions {

#define _OX_EPSILON_	0.000001f	// �덷
	struct HitResult
	{
		HitResult();

		HitResult(const Vec2& vec);

		~HitResult() = default;
		
		operator bool() const { return hit; };

	private:
		//����|�C���^�̂ق��������ł����H
		Vec2 pos;
		//null�Ȃ�݂����ȏ�����������������
		bool hit;
	};

	const HitResult NoHit;
	
	HitResult isHit(const Vec2& vec);

    // �����֌W�ɂ���H
	inline bool isSharpAngle(const Vec2& p1, const Vec2& p2, const Vec2& p3) {
		return (p1 - p2).dot(p3 - p2) >= 0.0f;
	}

    // �_�ƒ����̍ŒZ�����i2D�Łj
    // p : �_
    // l : ����
    // h : �_���牺�낵�������̑��i�߂�l�j
    // t :�x�N�g���W���i�߂�l�j
    // �߂�l: �ŒZ����
    float calcPointLineDist2D(const Vec2& p, const Line2D& l, Vec2& h, float& t);

    float calcPointSegmentDist2D(const Vec2& p, const Segment2D& seg, Vec2& h, float& t);

    // 0�`1�̊ԂɃN�����v
    inline void clamp01(float& v) {
        if (v < 0.0f)
            v = 0.0f;
        else if (v > 1.0f)
            v = 1.0f;
    }

    inline bool isParallel(Vec2 v1, Vec2 v2) {
            float d = v1.CCW(v2);
            return (-_OX_EPSILON_ < d&& d < _OX_EPSILON_);	// �덷�͈͓��Ȃ畽�s�Ɣ��� 
    }

    // 2�����̍ŒZ�����i2D�Łj
// l1 : L1
// l2 : L2
// p1 : L1���̐����̑��i�߂�l�j
// p2 : L2���̐����̑��i�߂�l�j
// t1 : L1���̃x�N�g���W���i�߂�l�j
// t2 : L2���̃x�N�g���W���i�߂�l�j
// �߂�l: �ŒZ����
    float calcLineLineDist2D(const Line2D& l1, const Line2D& l2, Vec2& p1, Vec2& p2, float& t1, float& t2);

    // 2�����̍ŒZ�����i2D�Łj
// s1 : S1(����1)
// s2 : S2(����2)
// p1 : S1���̐����̑��i�߂�l�j
// p2 : S2���̐����̑��i�߂�l�j
// t1 : S1���̃x�N�g���W���i�߂�l�j
// t2 : S2���̃x�N�g���W���i�߂�l�j
// �߂�l: �ŒZ����
    float calcSegmentSegmentDist2D(const Segment2D& s1, const Segment2D& s2, Vec2& p1, Vec2& p2, float& t1, float& t2);
}