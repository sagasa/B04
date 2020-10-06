#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <GStype.h>
#include <GSquaternion.h>
#include <GSvector3.h>
#include <GSmatrix4.h>
#include <list>

// トランスフォームクラス
class GStransform {
public:
	// 座標系の指定
	enum class Space {
		Self,   // ローカル座標系
		World   // ワールド座標系
	};
	// コンストラクタ
	GStransform() = default;
	// デストラクタ
	~GStransform();
	// ワールド空間の Transform の青軸を取得
	GSvector3 forward() const;
	// ワールド空間の Transform の青軸を設定
	void forward(const GSvector3& value);
	// ワールド空間の Transform の赤軸を取得
	GSvector3 right() const;
	// ワールド空間の Transform の赤軸を設定
	void right(const GSvector3& value);
	// ワールド空間の Transform の緑軸を取得
	GSvector3 up() const;
	// ワールド空間の Transform の緑軸を設定
	void up(const GSvector3& value);

	// オブジェクトのグローバルスケールを取得
	GSvector3 lossyScale() const;
	// ワールド空間の Transform の位置を取得
	GSvector3 position() const;
	// ワールド空間の Transform の位置を設定
	void position(const GSvector3& value);
	// GSquaternion として保存されるワールド空間での Transform の回転を取得
	GSquaternion rotation() const;
	// GSquaternion として保存されるワールド空間での Transform の回転を設定
	void rotation(const GSquaternion& value);
	// オイラー角としての角度を取得
	GSvector3 eulerAngles() const;
	// オイラー角としての角度を設定
	void eulerAngles(const GSvector3& value);
    void eulerAngles(float pitch, float yaw, float roll);

	// 対象の Transform を設定し、その方向へと向かせます
	void lookAt(const GStransform& target, const GSvector3& world_up = GSvector3{ 0.0f, 1.0f, 0.0f });
	void lookAt(const GSvector3& target, const GSvector3& world_up = GSvector3{ 0.0f, 1.0f, 0.0f });
	// Z 軸でeulers.z 度回転、X軸で euler.x 度回転、Y軸でeulers.y 度回転します(順番は説明した順)
	void rotate(const GSvector3& eulers, Space relative_to = Space::Self);
    void rotate(float pitch, float yaw, float roll, Space relative_to = Space::Self);
    // axis 軸の周りに angle 度、回転します
	void rotate(const GSvector3& axis, float angle, Space relative_to = Space::Self);
	// ワールド座標の point を中心とした軸(axis)で angle 度回転させます
	void rotateAround(const GSvector3& point, const GSvector3& axis, float angle);

	// translation の方向と距離に移動します
    void translate(float x, float y, float z, Space relative_to = Space::Self);
    void translate(const GSvector3& translation, Space relative_to = Space::Self);
    void translate(float x, float y, float z, const GStransform& relative_to);
    void translate(const GSvector3& translation, const GStransform& relative_to);

	// ローカル座標からワールド座標へ変換した行列
	GSmatrix4 localToWorldMatrix() const;
	// ワールド座標からローカル座標へ変換した行列
	GSmatrix4 worldToLocalMatrix() const;

	// ローカル空間からワールド空間へ position を変換します。(スケール・回転・平行移動）
	GSvector3 transformPoint(const GSvector3& position) const;
	// ローカル空間からワールド空間へ vector を変換します。(スケール・回転）
	GSvector3 transformVector(const GSvector3& vector) const;
	// ローカル空間からワールド空間へ direction を変換します(回転）
	GSvector3 transformDirection(const GSvector3& direction) const;

	// ワールド空間からローカル空間へ position を変換します
	GSvector3 inverseTransformPoint(const GSvector3& position) const;
	// ワールド空間からローカル空間へ vector を変換します
	GSvector3 inverseTransformVector(const GSvector3& vector) const;
	// ワールド空間からローカル空間へ direction を変換します
	GSvector3 inverseTransformDirection(const GSvector3& direction) const;

	// Transform の親を設定
	void parent(GStransform* parent);
	// Transform の親を取得
	GStransform* parent() const;
	// Transform の親を設定
	void setParent(GStransform* parent, bool world_position_stays = true);

	// 親の Transform オブジェクトから見た相対的なスケールを取得
	GSvector3 localScale() const;
	// 親の Transform オブジェクトから見た相対的なスケールを設定
	void localScale(const GSvector3& value);
	// 親の Transform オブジェクトから見た相対的な位置を取得
	GSvector3 localPosition() const;
	// 親の Transform オブジェクトから見た相対的な位置を設定
	void localPosition(const GSvector3& value);
	// 親の Transform オブジェクトから見た相対的な回転を取得
	GSquaternion localRotation() const;
	// 親の Transform オブジェクトから見た相対的な回転を設定
	void localRotation(const GSquaternion& value);
	// 親の Transform オブジェクトから見た相対的なオイラー角としての回転値を取得
	GSvector3 localEulerAngles() const;
	// 親の Transform オブジェクトから見た相対的なオイラー角としての回転値を設定
	void localEulerAngles(const GSvector3& value);
	// すべての子オブジェクトを親オブジェクトから切り離します
	void detachChildren();
	// 親の Transform から切り離す
	void detachParent();

	// 持ち主の取得
	void* owner() const;
	// 持ち主の設定
	void owner(void* p);

private:
	// ワールド座標系の更新
	void updateWorldTransform(const GStransform* parent);

private:
	// ポジション (ワールド座標系)
	GSvector3 position_{ 0.0f, 0.0f, 0.0f };
	// 回転 (ワールド座標系)
	GSquaternion rotation_{ 0.0f, 0.0f, 0.0f, 1.0f };
	// スケール (ワールド座標系)
	GSvector3 scale_{ 1.0f, 1.0f, 1.0f };

	// ポジション (ローカル座標系)
	GSvector3 local_position_{ 0.0f, 0.0f, 0.0f };
	// 回転 (ローカル座標系)
	GSquaternion local_rotation_{ 0.0f, 0.0f, 0.0f, 1.0f };
	// スケール (ローカル座標系)
	GSvector3 local_scale_{ 1.0f, 1.0f, 1.0f };

	// 親のトランスフォーム
	GStransform* parent_{ nullptr };
	// 子のトランスフォーム
	std::list<GStransform*> children_;

	// このトランスフォームの持ち主
	void* owner_{ nullptr };
};

#endif
