#include <GStransform.h>

// デストラクタ
GStransform::~GStransform() {
	// 子オブジェクトを切り離す
	detachChildren();
	// 親オブジェトから切り離す
	detachParent();
}

// ワールド空間の GStransformの青軸を取得
GSvector3 GStransform::forward() const {
	return rotation_ * GSvector3::forward();
}

// ワールド空間の GStransformの青軸を設定
void GStransform::forward(const GSvector3& value) {
	rotation(GSquaternion::lookRotation(value));
}

// ワールド空間の GStransform の赤軸を取得
GSvector3 GStransform::right() const {
	return rotation_ * GSvector3::right();
}

// ワールド空間の GStransform の赤軸を設定
void GStransform::right(const GSvector3& value) {
	rotation(GSquaternion::fromToRotation(GSvector3::right(), value));
}

// ワールド空間の GStransform の緑軸を取得
GSvector3 GStransform::up() const {
	return rotation_ * GSvector3::up();
}

// ワールド空間の GStransform の緑軸を設定
void GStransform::up(const GSvector3& value) {
	rotation(GSquaternion::fromToRotation(GSvector3::up(), value));
}

// オブジェクトのグローバルスケールを取得
GSvector3 GStransform::lossyScale() const {
	return scale_;
}

// ワールド空間の GStransform の位置を取得
GSvector3 GStransform::position() const {
	return position_;
}

// ワールド空間の GStransform の位置を設定
void GStransform::position(const GSvector3& value) {
	if (parent_ != nullptr) {
		localPosition(parent_->inverseTransformPoint(value));
	}
	else {
		localPosition(value);
	}
}

// GSquaternion として保存されるワールド空間での GStransform の回転を取得
GSquaternion GStransform::rotation() const {
	return rotation_;
}

// GSquaternion として保存されるワールド空間での GStransform の回転を設定
void GStransform::rotation(const GSquaternion& value) {
	if (parent_ != nullptr) {
		localRotation(GSquaternion::inverse(parent_->rotation()) * value);
	}
	else {
		localRotation(value);
	}
}

// オイラー角としての角度を取得
GSvector3 GStransform::eulerAngles() const {
	return rotation_.eulerAngles();
}

// オイラー角としての角度を設定
void GStransform::eulerAngles(const GSvector3& value) {
	rotation(GSquaternion::euler(value));
}

// オイラー角としての角度を設定
void GStransform::eulerAngles(float pitch, float yaw, float roll) {
    eulerAngles(GSvector3{ pitch, yaw, roll });
}

// 対象の GStransform を設定し、その方向へと向かせます
void GStransform::lookAt(const GStransform& target, const GSvector3& world_up) {
	lookAt(target.position(), world_up);
}

// 対象の GStransform を設定し、その方向へと向かせます
void GStransform::lookAt(const GSvector3& target, const GSvector3& world_up) {
	rotation(GSquaternion::lookRotation(target - position_, world_up));
}

// Z 軸でeulers.z 度回転、X軸で euler.x 度回転、Y軸でeulers.y 度回転します(順番は説明した順)
void GStransform::rotate(const GSvector3& eulers, Space relative_to) {
	if (relative_to == Space::Self) {
		localRotation(local_rotation_ * GSquaternion::euler(eulers.x, eulers.y, eulers.z));
	} else {
		rotation(GSquaternion::euler(eulers.x, eulers.y, eulers.z) * rotation_);
	}
}

// Z 軸でroll 度回転、X軸で pitch 度回転、Y軸でyow 度回転します(順番は説明した順)
void GStransform::rotate(float pitch, float yaw, float roll, Space relative_to) {
    rotate(GSvector3{ pitch, yaw, roll }, relative_to);
}

// axis 軸の周りに angle 度、回転します
void GStransform::rotate(const GSvector3& axis, float angle, Space relative_to) {
	if (relative_to == Space::Self) {
		localRotation(local_rotation_ * GSquaternion::angleAxis(angle, axis));
	} else {
		rotation(GSquaternion::angleAxis(angle, axis) * rotation_);
	}
}

// ワールド座標の point を中心とした軸(axis)で angle 度回転させます
void GStransform::rotateAround(const GSvector3& point, const GSvector3& axis, float angle) {
	GSquaternion q = GSquaternion::angleAxis(angle, axis);
	position(point + q * (position_ - point));
	rotation(q * rotation_);
}

// translation の方向と距離に移動します
void GStransform::translate(float x, float y, float z, Space relative_to) {
    translate(GSvector3{ x, y, z }, relative_to);
}

// translation の方向と距離に移動します
void GStransform::translate(const GSvector3& translation, Space relative_to) {
	if (relative_to == Space::Self) {
		position(position_ + transformDirection(translation));
	} else {
		position(position_ + translation);
	}
}

// translation の方向と距離に移動します
void GStransform::translate(const GSvector3& translation, const GStransform& relative_to) {
   position(position_ + relative_to.transformDirection(translation));
}

// translation の方向と距離に移動します
void GStransform::translate(float x, float y, float z, const GStransform& relative_to) {
    translate(GSvector3{ x, y, z }, relative_to);
}

// ローカル座標からワールド座標へ変換した行列
GSmatrix4 GStransform::localToWorldMatrix() const {
	return GSmatrix4::TRS(position_, rotation_, scale_);
}

// ワールド座標からローカル座標へ変換した行列
GSmatrix4 GStransform::worldToLocalMatrix() const {
	return localToWorldMatrix().inverse();
}

// ローカル空間からワールド空間へ position を変換します (スケール・回転・平行移動）
GSvector3 GStransform::transformPoint(const GSvector3& position) const {
	return rotation_ * GSvector3::scale(position, scale_) + position_;
}

// ローカル空間からワールド空間へ vector を変換します (スケール・回転）
GSvector3 GStransform::transformVector(const GSvector3& vector) const {
	return rotation_ * GSvector3::scale(vector, scale_);
}

// ローカル空間からワールド空間へ direction を変換します(回転）
GSvector3 GStransform::transformDirection(const GSvector3& direction) const {
	return rotation_ * direction;
}

// ワールド空間からローカル空間へ position を変換します
GSvector3 GStransform::inverseTransformPoint(const GSvector3& position) const {
	GSvector3 result = position;
	result -= position_;
	result = GSquaternion::inverse(rotation_) * result;
	result.x /= scale_.x;
	result.y /= scale_.y;
	result.z /= scale_.z;
	return result;
}

// ワールド空間からローカル空間へ vector を変換します
GSvector3 GStransform::inverseTransformVector(const GSvector3& vector) const {
	GSvector3 result = vector;
	result = GSquaternion::inverse(rotation_) * result;
	result.x /= scale_.x;
	result.y /= scale_.y;
	result.z /= scale_.z;
	return result;
}

// ワールド空間からローカル空間へ direction を変換します
GSvector3 GStransform::inverseTransformDirection(const GSvector3& direction) const {
	return GSquaternion::inverse(rotation_) * direction;
}

// 親の GStransform を取得
GStransform* GStransform::parent() const {
	return parent_;
}

// 親の GStransform を設定
void GStransform::parent(GStransform* parent) {
	setParent(parent, true);
}

// 親の GStransform を設定
void GStransform::setParent(GStransform* parent, bool world_position_stays) {
	// 現在の親から切り離す
	detachParent();
	// 新しい親を設定
	parent_ = parent;
	if (parent_ != nullptr) {
		// 現在のワールド空間を保つか？
		if (world_position_stays) {
			// ローカル空間だけ更新する
			local_rotation_ = GSquaternion::inverse(parent_->rotation()) * rotation_;
			local_position_ = parent_->inverseTransformPoint(position_);
		}
		else {
			// 現在のローカル空間を更新せずにワールド空間を更新
			updateWorldTransform(parent_);
		}
		// 自分自身を親の子に登録
		parent_->children_.push_back(this);
	}
	else {
		// 親がいなければ、ローカル空間はワールド空間と同じ
		local_rotation_ = rotation_;
		local_position_ = position_;
		local_scale_ = scale_;
	}
}

// 親の GStransform オブジェクトから見た相対的なスケールを取得
GSvector3 GStransform::localScale() const {
	return local_scale_;
}

// 親の GStransform オブジェクトから見た相対的なスケールを設定
void GStransform::localScale(const GSvector3& value) {
	local_scale_ = value;
	// ワールド空間を更新
	updateWorldTransform(parent_);
}

// 親の GStransform オブジェクトから見た相対的な位置を取得
GSvector3 GStransform::localPosition() const {
	return local_position_;
}

// 親の GStransform オブジェクトから見た相対的な位置を設定
void GStransform::localPosition(const GSvector3& value) {
	local_position_ = value;
	// ワールド空間を更新
	updateWorldTransform(parent_);
}

// 親の GStransform オブジェクトから見た相対的な回転を取得
GSquaternion GStransform::localRotation() const {
	return local_rotation_;
}

// 親の GStransform オブジェクトから見た相対的な回転を設定
void GStransform::localRotation(const GSquaternion& value) {
	local_rotation_ = value;
	// ワールド空間を更新
	updateWorldTransform(parent_);
}

// 親の GStransform オブジェクトから見た相対的なオイラー角としての回転値を取得
GSvector3 GStransform::localEulerAngles() const {
	return local_rotation_.eulerAngles();
}

// 親の GStransform オブジェクトから見た相対的なオイラー角としての回転値を設定
void GStransform::localEulerAngles(const GSvector3& value) {
	localRotation(GSquaternion::euler(value));
}

// すべての子オブジェクトを親オブジェクトから切り離します
void GStransform::detachChildren() {
	// 自身の親を子の親に変更する
	for (auto child : children_) {
		child->parent(parent_);
	}
}

// 親の GStransform から切り離す
void GStransform::detachParent() {
	if (parent_ != nullptr) {
		// 親のリストから自身を削除
		parent_->children_.remove(this);
		parent_ = nullptr;
	}
}

// 持ち主の取得
void * GStransform::owner() const {
	return owner_;
}

// 持ち主の設定
void GStransform::owner(void * p) {
	owner_ = p;
}

// ワールド空間の更新
void GStransform::updateWorldTransform(const GStransform* parent) {
	if (parent != nullptr) {
		// ワールド空間の更新
		position_ = parent->transformPoint(local_position_);
		rotation_ = parent->rotation_ * local_rotation_;
		scale_ = GSvector3::scale(parent->scale_, local_scale_);
	} else {
		// 親がいなければローカル空間とワールド空間は同じ
		position_ = local_position_;
		rotation_ = local_rotation_;
		scale_ = local_scale_;
	}
	// 子供のワールド空間を更新
	for (auto child : children_) {
		child->updateWorldTransform(this);
	}
}