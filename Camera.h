#pragma once
#include "Actor.h"


// カメラクラス
class Camera : public Actor {
public:
    // コンストラクタ
    Camera(IWorld* world);
    //更新
    virtual void update(float delta_time)override;
    // 描画
    virtual void draw() const override;

private:
    Actor* player_;
};