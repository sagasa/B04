#include "ResultScene.h"
#include"Assets.h"
#include"SceneManager.h"
#include<GSmusic.h>


//開始
void ResultScene::start() {
	is_end_ = false;
	select_ = Select::Next;
	//テクスチャの読み込み
	gsLoadTexture(Texture_Clear, "Assets/Image/clear.dds");
	gsLoadTexture(Texture_Return_To_Title, "Assets/Image/return_to_title.dds");
	gsLoadTexture(Texture_Next_Stage, "Assets/Image/next_stage.dds");
	gsLoadTexture(Texture_One_More, "Assets/Image/one_more.dds");
}

//更新
void ResultScene::update(float delta_time) {
	//ワールドの更新
	//world_.update(delta_time);
	//キーを押したらタイトルへ
	if (gsGetKeyTrigger(GKEY_RETURN)) {
		is_end_ = true;
	}

}

//描画
void ResultScene::draw() const {
	//ワールドの描画
	//world_.draw();
	gsDrawSprite2D(Texture_Clear, &GSvector2{ 500,50 }, NULL, NULL, NULL, NULL, NULL);
	gsDrawSprite2D(Texture_Next_Stage, &GSvector2{ 350,250 }, NULL, NULL, NULL, &GSvector2{ 0.5,0.5 }, NULL);
	gsDrawSprite2D(Texture_One_More, &GSvector2{ 450,400 }, NULL, NULL, NULL, &GSvector2{ 0.5,0.5 }, NULL);
	gsDrawSprite2D(Texture_Return_To_Title, &GSvector2{ 450,550 }, NULL, NULL, NULL, &GSvector2{0.5,0.5}, NULL);
	
}

//終了しているか？
bool ResultScene::is_end() const {
	return is_end_;
}

//次のシーンを返す
std::string ResultScene::next() const {
	return "TitleScene";
}

//終了
void ResultScene::end() {
	gsDeleteTexture(Texture_Clear);
	gsDeleteTexture(Texture_Return_To_Title);
	gsDeleteTexture(Texture_Next_Stage);
	gsDeleteTexture(Texture_One_More);
}