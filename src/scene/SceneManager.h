#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include<unordered_map>
#include<string>

#include "resource_loader.h"

class IScene;
//シーン管理クラス
class SceneManager {
public:
	//コンストラクタ
	SceneManager();
	//デストラクタ
	~SceneManager();
	//更新
	void update(float delta_time);
	//描画
	void draw() const;
	//終了
	void end();
	//シーンの追加
	void add(const std::string& name, IScene* scene);
	//シーンの変更
	void change(const std::string& name);
	//シーンの消去
	void clear();
	//コピー禁止
	SceneManager(const SceneManager& other) = delete;
	SceneManager& operator = (const SceneManager& other) = delete;
private:
	//読み込み完了 次のシーンを実行
	std::function<void()> load_end_;
	//シーン
	std::unordered_map<std::string, IScene* > scenes_;
	resource_loader loader_;
	IScene* next_scene_;
	IScene* current_scene_;
	//現在のステージ番号
	int stage_number_ = 1;
};

#endif
