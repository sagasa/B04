#ifndef ISCENE_H_
#define ISCENE_H_

#include<string>
#include<GSmusic.h>

class resource_loader;

//シーン抽象インターフェース
class IScene {
public:
	//仮想デストラクタ
	virtual ~IScene() = default;
	//読み込み開始
	virtual void load(resource_loader& loader) {}
	//開始
	virtual void start(int stage_number = 1) = 0;
	//更新
	virtual void update(float delta_time) = 0;
	//描画
	virtual void draw() const = 0;
	//終了しているか？
	virtual bool is_end() const = 0;
	//次のシーン名を返す
	virtual std::string next() const = 0;
	//現在のステージ番号を返す
	virtual int stage_number() const = 0;
	//終了
	virtual void end() = 0;

protected:
	int stage_number_ = 1;
};

#endif