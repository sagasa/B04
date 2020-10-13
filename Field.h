#pragma once
#include <gslib.h>

// フィールドクラス
class Field {
public:
    // コンストラクタ
    Field(GSuint bg);
    // 更新
    void update(float delta_time);
    // 描画
    void draw() const;
    // フィールド内か？
    bool is_inside(const GSvector3& position) const;
    // フィールド外か？
    bool is_outside(const GSvector3& position) const;

    // コピー禁止
    Field(const Field& other) = delete;
    Field& operator = (const Field& other) = delete;
private:
    // 背景画像
    GSuint  bg_{ 0 };
    // スクロール位置
    float   scroll_{ 0.0f };
};
