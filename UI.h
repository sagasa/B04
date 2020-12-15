#ifndef UI_H_
#define UI_H_

#include"Actor.h"
#include"ActorProp.h"

//UIクラス
class UI : public Actor {
public:
	//コンストラクタ
	UI() = default;
	//デストラクタ
	~UI() = default;
	// GUIの描画
	virtual void draw_gui() const override;

public:
	ActorProp* player_;
};


#endif
