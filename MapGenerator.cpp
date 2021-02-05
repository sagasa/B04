#include"MapGenerator.h"
#include"IWorld.h"
#include"Player.h"
#include"CarGhost.h"
#include"RushGhost.h"
#include"Poltergeist.h"
#include"NormalGhost.h"
#include"SurogSakones.h"
#include"DeathSmoke.h"
#include"player_paladin.h"

const float size_{ 1 };

//コンストラクタ
MapGenerator::MapGenerator(IWorld* world, const std::string& file_name) {
	csv_.load(file_name);//CSVファイルの読み込み
	//ワールドの設定
	world_ = world;
	//名前の設定
	name_ = "MapGenerator";
	tag_ = "generatorTag";
	enable_collider_ = false;
	while (current_row_ < csv_.rows() - 1) {
		current_row_++;
		//座標を取得
		GSvector3 position{
			csv_.getf(current_row_,size_ + 0),
			csv_.getf(current_row_,size_ + 1),
			csv_.getf(current_row_,size_ + 2)
		};
		generate(world_, position);
	}
	current_row_++;
}

//更新
void MapGenerator::update(float delta_time) {

	//if (current_row_ < csv_.rows()) {
	//	//プレイヤーを検索
	//	player_ = world_->find_actor("Player");
	//	if (player_ == nullptr) {
	//		player_ = world_->find_actor("PlayerPaladin");
	//		if (player_ == nullptr) return;
	//	}

	//	//座標を取得
	//	GSvector3 position{
	//		csv_.getf(current_row_,size_ + 0),
	//		csv_.getf(current_row_,size_ + 1),
	//		csv_.getf(current_row_,size_ + 2)
	//	};

	//	if (player_->transform().position().x + 20.0f >= position.x) {
	//		generate(world_, position);
	//		current_row_++;
	//	}
	//}
	
}

//生成
void MapGenerator::generate(IWorld* world,const GSvector3& position) {
	
	//文字を取得
	std::string name = csv_.get(current_row_,0);

	//生成処理
	if (name == "NormalGhost") {
		world->add_actor(new NormalGhost{ world,position });
	}
	else if (name == "CarGhost") {
		world->add_actor(new CarGhost{ world,position });
	}
	else if (name == "Poltergeist") {
		world->add_actor(new Poltergeist{ world,position });
	}
	else if (name == "RushGhost") {
		world->add_actor(new RushGhost{ world,position });
	}
	else if (name == "Paladin") {
		world->add_actor(new player_paladin{ world,position });
	}
	else if (name == "SurogSakones")
	{
		world->add_actor(new SurogSakones{ world,position });
	}
	else if (name == "DeathSmoke")
	{
		world->add_actor(new DeathSmoke{ world,position });
	}
}