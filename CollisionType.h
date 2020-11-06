#pragma once
#include <GSgame.h>
#include <iostream>

namespace collisions {
    struct DebugView {
        virtual void drawDebug() = 0;
    };

    typedef GSvector2 Vec2;

    struct Line2D : public DebugView {
        Vec2 p;
        Vec2 v;		// �����x�N�g��
        Line2D() : p(0.0f, 0.0f), v(1.0f, 0.0f) {}
        Line2D(const Vec2& p, const Vec2& v) : p(p), v(v) {}
        ~Line2D() = default;

        // �_��̍��W���擾
        //  �x�N�g���Ɋ|���Z����W��
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

        bool isRight(const Vec2& pos) const
        {
            return (pos - p).CCW(v)<0;
        }

        // �I�_���擾
        Vec2 getEndPoint() const {
            return p + v;
        }
    };

    struct Collision {
        //virtual Collision transform(const GSmatrix4& matrix) const;
        virtual ~Collision() = default;
    };

    struct Circle2D :public Collision {
        Vec2 p;
        float r;	// ���a
        Circle2D() : p(0.0f, 0.0f), r(0.5f) {}
        Circle2D(const Vec2& p, float r) : p(p), r(r) {}
        ~Circle2D() = default;
    };

    struct Capsule2D :public Collision {
        Segment2D s;
        float r;	// ���a
        Capsule2D() : r(0.5f) {}
        Capsule2D(const Segment2D& s, float r) : s(s), r(r) {}
        Capsule2D(const Vec2& p1, const Vec2& p2, float r) : s(p1, p2), r(r) {}
        ~Capsule2D() = default;
    };
    struct Box2D :public Collision {
        Vec2 p;
        Vec2 size;
        float r;	// ��]
        Box2D() : r(0) {}
        Box2D(const Vec2& pos, const Vec2& s, float r) : p(pos), size(s), r(r) {}

        static bool isRight(const Vec2& pos, const Vec2& other) 
        {
            return (pos ).CCW(other) <= 0;
        }
    	
        bool isInside(const Vec2& vec)const
        {
            //std::cout << vec.x << " " << vec.y << p.x << " " << p.y << std::endl;
            const Vec2 pos = vec - p;
            if (!isRight(size * Vec2{ 0,1 }, pos + size * Vec2{ 0.5,0.5 }))
                return false;
            if (!isRight(size * Vec2{ 1,0 }, pos + size * Vec2{ 0.5,-0.5 }))
                return false;
            if (!isRight(size * Vec2{ 0,-1 }, pos + size * Vec2{ -0.5,-0.5 }))
                return false;
            if (!isRight(size * Vec2{ -1,0 }, pos + size * Vec2{ -0.5,0.5 }))
                return false;
            return true;
        }

        Box2D transform(const GSmatrix4& matrix) const{
            return Box2D{ matrix.transform(p),matrix.getScale().xy*size, r };
        }
    	
        ~Box2D() = default;
    };
}
