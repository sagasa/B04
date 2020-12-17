#ifndef UI_H_
#define UI_H_

#include"Actor.h"
#include"ActorProp.h"

//UIクラス
class UI : public Actor {
public:
	//コンストラクタ
	UI(IWorld* world);
	//アップデート
	void update(float delta_time) override;
	// GUIの描画
	virtual void draw_gui() const override;

private:
	void change_gui();

private:
	Actor* player_{nullptr};
	int hp_{ 0 };
	int shield_hp_{ 0 };
};


#endif
