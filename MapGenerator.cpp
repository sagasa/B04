#include"MapGenerator.h"
#include"IWorld.h"
#include"Player.h"
#include"CarGhost.h"
#include"RushGhost.h"
#include"Poltergeist.h"
#include"SurogSakones.h"

const int CsvTime{ 0 };    //タイマーの列
const int CsvName{ 1 };    //名前の列
const int CsvPosition{ 2 };//座標の列
const int CsvVelocity{ 5 };//移動量の列

const float size_{ 1 };

//コンストラクタ
MapGenerator::MapGenerator(IWorld* world, const std::string& file_name) {
	world_ = world;
	name_ = "EnemyGenerator";
	tag_ = "Generator";
	csv_.load(file_name);//CSVファイルの読み込み
}

//更新
void MapGenerator::update(float delta_time) {
	for (int column = 0;column < csv_.columns(); column++) {
		for (int row = 0;row < csv_.rows(); row++) {
			generate(row,column);
		}
	}
	//CSVのデータがまだある && 出現時間であれば
	while (current_row_ < csv_.rows()) {
		//敵の生成
		//generate();
		//CSVデータの読み込み行を更新
		++current_row_;
	}
	
}

//生成
void MapGenerator::generate(int row,int column) {
	//名前を取得
	std::string name = csv_.get(row, column);
	//座標を取得
	GSvector3 position{
		column * size_,
		row * size_,
		0.0f
	};
	//キャラクタの生成
	if (name == "Block") {
		//ブロックの生成
	}else if (name == "Player") {
		world_->add_actor(new Player{world_,position });
	}else if (name == "CarGhost") {
		world_->add_actor(new CarGhost{ world_,position });
	}
	else if (name == "RushGhost") {
		world_->add_actor(new RushGhost{ world_,position });
	}
	else if (name == "Poltergeist") {
		world_->add_actor(new Poltergeist{ world_,position });
	}
	else if (name == "SurogSakones") {
		world_->add_actor(new SurogSakones{ world_,position });
	}
	
}