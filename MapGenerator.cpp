#include"MapGenerator.h"
#include"IWorld.h"
#include"Player.h"
#include"CarGhost.h"
#include"RushGhost.h"
#include"Poltergeist.h"
#include"SurogSakones.h"

const float size_{ 1 };

//コンストラクタ
MapGenerator::MapGenerator(IWorld* world, const std::string& file_name) {
	csv_.load(file_name);//CSVファイルの読み込み
	for (int row = 0; row < csv_.rows(); row++) {
			generate(world,row, 0);
	}
}

//生成
void MapGenerator::generate(IWorld* world ,int row, int column) {
	
	//数字を取得
	std::string num = csv_.get(row, column);
	//座標を取得
	GSvector3 position{
		csv_.getf(row,1),
		csv_.getf(row,2),
		0.0f
	};


	 if (num == "Player") {//プレイヤー
		world->add_actor(new Player{ world,position });
	}
	else if (num == "CarGhost") {//CarGhost
		world->add_actor(new CarGhost{ world,position });
	}
	else if (num == "RushGhost") {//RushGhost
		world->add_actor(new RushGhost{ world,position });
	}
	else if (num == "Poltergeist") {//Poltergeist
		world->add_actor(new Poltergeist{ world,position });
	}
	else if (num == "SurogSakones") {//SurogSakones
		world->add_actor(new SurogSakones{ world,position });
	}

}