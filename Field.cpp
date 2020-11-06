#include "Field.h"

// フィールドのサイズ
const float FieldSize{ 150.0f };

// コンストラクタ
Field::Field(GSuint bg) : bg_{ bg }, scroll_{ 0.0f } {
}

// 更新
void Field::update(float delta_time) {
}

// 描画
void Field::draw() const {
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