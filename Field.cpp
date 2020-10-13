#include "Field.h"

// フィールドのサイズ
const float FieldSize{ 150.0f };

// コンストラクタ
Field::Field(GSuint bg) : bg_{ bg }, scroll_{ 0.0f } {
}

// 更新
void Field::update(float delta_time) {
    // スクロール値の更新
    scroll_ -= 0.2f * delta_time;
}

// 描画
void Field::draw() const {
    // 背景画像の描画
    const GSrect src_rect{ 0.0f, scroll_, 640.0f, 480.0f + scroll_ };
    gsDrawSprite2D(bg_, NULL, &src_rect, NULL, NULL, NULL, 0.0f);
}

// フィールド内か？
bool Field::is_inside(const GSvector3& position) const {
    if (ABS(position.x) >= FieldSize) return false;
    if (ABS(position.y) >= FieldSize) return false;
    return true;
}

// フィールド外か？
bool Field::is_outside(const GSvector3& position) const {
    return !is_inside(position);
}