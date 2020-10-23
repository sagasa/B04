#pragma once
#include <GSgame.h>

namespace collisions {
    typedef GSvector2 Vec2;

    struct Line2D {
        Vec2 p;
        Vec2 v;		// �����x�N�g��
        Line2D() : p(0.0f, 0.0f), v(1.0f, 0.0f) {}
        Line2D(const Vec2& p, const Vec2& v) : p(p), v(v) {}
        ~Line2D() {}

        // �_��̍��W���擾
        //  �x�N�g���Ɋ|���Z����W��
        Vec2 getPoint(float t) const {
            return p + t * v;
        }
    };

    struct Segment2D :Line2D
    {
        Segment2D() {}
        Segment2D(const Vec2& p, const Vec2& v) : Line2D(p, v) {}

        // �I�_���擾
        Vec2 getEndPoint() const {
            return p + v;
        }
    };

    struct Circle2D {
        Vec2 p;
        float r;	// ���a
        Circle2D() : p(0.0f, 0.0f), r(0.5f) {}
        Circle2D(const Vec2& p, float r) : p(p), r(r) {}
        ~Circle2D() {}
    };

    struct Capsule2D {
        Segment2D s;
        float r;	// ���a
        Capsule2D() : r(0.5f) {}
        Capsule2D(const Segment2D& s, float r) : s(s), r(r) {}
        Capsule2D(const Vec2& p1, const Vec2& p2, float r) : s(p1, p2), r(r) {}
        ~Capsule2D() {}
    };
}