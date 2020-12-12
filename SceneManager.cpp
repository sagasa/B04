#include"SceneManager.h"

#include <iostream>

#include"SceneNull.h"

static SceneNull scene_null;

//コンストラクタ
SceneManager::SceneManager() :
	current_scene_{ &scene_null } {
	load_end_ = [this] {
		//次のシーンを開始 戻るまでアップデートしないように
		next_scene_->start();
		current_scene_ = next_scene_;
		next_scene_ = nullptr;
	};
}

//デストラクタ
SceneManager::~SceneManager() {
	clear();
}
//更新
void SceneManager::update(float delta_time) {
	//シーンの更新
	current_scene_->update(delta_time);
	//ロードが終わっていれば
	if (next_scene_!=nullptr&&loader_.count_ == 0)
	{
		load_end_();
	}

	//シーンが終了しているか？
	if (current_scene_->is_end()) {
		//シーンを変更する
		change(current_scene_->next());
	}
}

//描画
void SceneManager::draw() const {
	//現在のシーンを描画
	current_scene_->draw();
}

//終了
void SceneManager::end() {
	//現在のシーンを終了
	current_scene_->end();
	//nullシーンにしておく
	current_scene_ = &scene_null;
}

//シーンの追加
void SceneManager::add(const std::string& name, IScene* scene) {
	scenes_[name] = scene;
}

//シーンの変更
void SceneManager::change(const std::string& name) {
	//現在のシーンを終了
	end();
	//現在のシーンを変更
	next_scene_ = scenes_[name];
	next_scene_->load(loader_);
	//綺麗に書きたかったです
	if(loader_.count_==0)
	{
		load_end_();
	}
	//loader_.on_end(load_end_);
}



//シーンの消去
void SceneManager::clear() {
	end();
	for (auto & pair : scenes_) {
		delete pair.second;
	}
	scenes_.clear();
}