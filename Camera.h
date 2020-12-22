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
    //見えているかどうか？
    bool is_viewing(const BoundingSphere& sphere);
    

private:
    //プレイヤー
    Actor* player_;
    //固定にするかどうか？
    bool is_fixed_;

};