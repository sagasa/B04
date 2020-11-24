#ifndef POLTER_GEIST_H_
#define POLTER_GEIST_H_

#include"Actor.h"
#include"AnimatedMesh.h"

class Poltergeist : public Actor {
public:
	enum class State {
		Idle,
		Turn,
		Found,
		Attack,
		Damage,
		Died
	};
	//コンストラクタ
	Poltergeist(IWorld* world, const GSvector3& position);
	//更新
	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;
	//衝突リアクション
	virtual void react(Actor& other) override;

private:
	//状態更新
	void update_state(float delta_time);
	//状態変更
	void change_state(State state, GSuint motion,bool loop = true);
	//アイドル
	void idle(float delta_time);
	//プレイヤーを見つけた
	void found(float delta_time);
	//ターン
	void turn(float delta_time);
	//攻撃
	void attack(float delta_time);
	//ダメージ
	void damage(float delta_time);
	//死亡
	void died(float delta_time);

	//フィールドとの衝突処理
	void collide_field();
	//アクターとの衝突処理
	void collide_actor(Actor& other);

	//振り向き判定
	bool is_turn()const;
	//攻撃判定
	bool is_attack()const;

	//ターゲット方向の角度を求める(符号付き)
	float target_signed_angle()const;
	//ターゲット方向の角度を求める
	float target_angle()const;
	//ターゲットの距離を求める
	float target_distance()const;
	//ターゲットのxの距離を求める
	float target_distance_x() const;
	//target方向のyの距離を求める
	float target_distance_y() const;

	//ターゲット方向のベクトルを求める
	GSvector3 to_target()const;

private:
	//アニメーションメッシュ
	AnimatedMesh mesh_;
	//モーション番号
	GSuint motion_;
	//モーションのループ指定
	bool motion_loop_;
	//プレイヤー
	Actor* player_;
	//体力
	float hp_{ 1.0f };
	//移動タイマー
	float moving_timer_{ 0.0f };
	//射撃タイマー
	float shootiong_timer_{ 0.0f };
	//スピード
	float speed_{ 1.0f };
	//状態
	State state_;
	//状態タイマー
	float state_timer_;
};
#endif