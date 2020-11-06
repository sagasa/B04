#include "Field.h"

// �t�B�[���h�̃T�C�Y
const float FieldSize{ 150.0f };

// �R���X�g���N�^
Field::Field(GSuint bg) : bg_{ bg }, scroll_{ 0.0f } {
}

// �X�V
void Field::update(float delta_time) {
}

// �`��
void Field::draw() const {
}

// �t�B�[���h�����H
bool Field::is_inside(const GSvector3& position) const {
    if (ABS(position.x) >= FieldSize) return false;
    if (ABS(position.y) >= FieldSize) return false;
    return true;
}

// �t�B�[���h�O���H
bool Field::is_outside(const GSvector3& position) const {
    return !is_inside(position);
}