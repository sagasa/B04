#pragma once
#include <GSgame.h>

namespace collisions {
    struct DebugView {
        virtual void drawDebug() = 0;
    };

    typedef GSvector2 Vec2;

    struct Line2D : public DebugView {
        Vec2 p;
        Vec2 v;		// •ûŒüƒxƒNƒgƒ‹
        Line2D() : p(0.0f, 0.0f), v(1.0f, 0.0f) {}
        Line2D(const Vec2& p, const Vec2& v) : p(p), v(v) {}
        ~Line2D() {}

        // “_ã‚ÌÀ•W‚ğæ“¾
        //  ƒxƒNƒgƒ‹‚ÉŠ|‚¯Z‚·‚éŒW”
        Vec2 getPoint(float t) const {
            return p + t * v;
        }
 
        void drawDebug() {
            glBegin(GL_LINE);
        }
    };

    struct Segment2D :public Line2D
    {
        Segment2D() {}
        Segment2D(const Vec2& p, const Vec2& v) : Line2D(p, v) {}

        // I“_‚ğæ“¾
        Vec2 getEndPoint() const {
            return p + v;
        }
    };

    struct Circle2D {
        Vec2 p;
        float r;	// ”¼Œa
        Circle2D() : p(0.0f, 0.0f), r(0.5f) {}
        Circle2D(const Vec2& p, float r) : p(p), r(r) {}
        ~Circle2D() {}
    };

    struct Capsule2D {
        Segment2D s;
        float r;	// ”¼Œa
        Capsule2D() : r(0.5f) {}
        Capsule2D(const Segment2D& s, float r) : s(s), r(r) {}
        Capsule2D(const Vec2& p1, const Vec2& p2, float r) : s(p1, p2), r(r) {}
        ~Capsule2D() {}
    };
    struct Box2D {
        Vec2 size;
        float r;	// ‰ñ“]
        Box2D() : r(0.5f) {}
        Box2D(const Vec2& s, float r) : size(s), r(r) {}
        ~Box2D() {}
    };
}