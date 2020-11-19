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
        Vec2 v;		// •ûŒüƒxƒNƒgƒ‹
        Line2D() : p(0.0f, 0.0f), v(1.0f, 0.0f) {}
        Line2D(const Vec2& p, const Vec2& v) : p(p), v(v) {}
        ~Line2D() = default;

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

        bool isRight(const Vec2& pos) const
        {
            return (pos - p).CCW(v)<0;
        }

        // I“_‚ğæ“¾
        Vec2 getEndPoint() const {
            return p + v;
        }
    };

    struct physics_mat
    {
        float friction;
        float restitution;
        bool is_static;
        float weight;

    	physics_mat() :friction(0.5),restitution(0.5),is_static(false),weight(1){}

       
    };

    struct Collision {
        //virtual Collision transform(const GSmatrix4& matrix) const;//‚Ç‚¤‚·‚ê‚Î‚¢‚¢‚Ì‚³
        physics_mat default_mat;

        Collision(): mat(default_mat){}
        Collision(const physics_mat& mat) : mat(mat) {}
        physics_mat mat;
        virtual ~Collision() = default;
    };

    struct Circle2D :public Collision {
        Vec2 p;
        float r;	// ”¼Œa
        Circle2D(const Vec2& p, float r, const physics_mat& mat) :Collision(mat), p(p), r(r) {}
        ~Circle2D() = default;
    };

    struct Capsule2D :public Collision {
        Segment2D s;
        float r;	// ”¼Œa
        Capsule2D(const Segment2D& s, float r, const physics_mat& mat) : Collision(mat), s(s), r(r) {}
        Capsule2D(const Vec2& p1, const Vec2& p2, float r, const physics_mat& mat) :Collision(mat), s(p1, p2), r(r) {}
        ~Capsule2D() = default;
    };
	//‰ñ“]‚Í–¢À‘•
    struct Box2D :public Collision {
        Vec2 p;
        Vec2 size;
        float r;	// ‰ñ“]
        Box2D():p(Vec2{ 0,0 }), size(Vec2{ 1,1 }), r(0) {}
        Box2D(const Vec2& pos, const Vec2& size, const physics_mat& mat) :Collision(mat), p(pos), size(size), r(0) {}

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
            return Box2D{ matrix.transform(p),matrix.getScale().xy*size, mat };
        }
    	
        ~Box2D() = default;
    };
}
