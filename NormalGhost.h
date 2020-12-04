#ifndef NORMAL_GHOST_H_
#define NORMAL_GHOST_H_

#include"Actor.h"
#include"AnimatedMesh.h"

//ノーマルゴーストクラス
class NormalGhost : public Actor {
public:

	enum class State {
		Idle,
		Move,
		Damage,
		Died
	};

	//コンストラクタ
	NormalGhost(IWorld* world, const GSvector3& position);
	//更新
	virtual void update(float delta_time) override;
	//描画
	virtual void draw() const override;
	//衝突リアクション
	virtual void react(Actor& other) override;

private:

	//状態の更新
	void update_state(float delta_time);
	//状態の変更
	void change_state(State state, GSuint motion, bool loop = true);
	//アイドル
	void idle(float delta_time);
	//移動
	void move(float delta_time);
	//ダメージ中
	void damage(float delta_time);
	//死亡中
	void died(float delta_time);

	//フィールドとの衝突処理
	void collide_field();
	//アクターとの衝突処理
	void collide_actor(Actor& other);

	//移動判定
	bool is_move()const;

	//ターゲット方向の角度を求める(符号付き)
	float target_signed_angle()const;
	//ターゲット方向の角度を求める
	float target_angle()const;
	//ターゲットの距離を求める
	float target_distance()const;
	//ターゲット方向のベクトルを求める
	GSvector3 to_target()const;

private:
	///アニメーションメッシュ
	AnimatedMesh mesh_;
	//モーション番号
	GSuint motion_;
	//モーションのループ指定
	bool motion_loop_;
	//プレイヤー
	Actor* player_;
	//体力
	float hp_{ 1.0f };
	//タイマー
	float moving_timer_{ 0.0f };
	//状態
	State state_;
	//状態タイマー
	float state_timer_;
	
};


#endif

