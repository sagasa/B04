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
		for (int column = 0; column < csv_.columns(); column++) {
			generate(world,row, column);
		}
	}
}

//生成
void MapGenerator::generate(IWorld* world ,int row, int column) {
	
	//数字を取得
	int num = csv_.geti(row, column);
	//座標を取得
	GSvector3 position{
		column * size_,
		row * size_,
		0.0f
	};

	if (num == 0) {//地面
		//ブロックの生成
	}
	else if (num == 1) {//プレイヤー
		world->add_actor(new Player{ world,position });
	}
	else if (num == 2) {//CarGhost
		world->add_actor(new CarGhost{ world,position });
	}
	else if (num == 3) {//RushGhost
		world->add_actor(new RushGhost{ world,position });
	}
	else if (num == 4) {//Poltergeist
		world->add_actor(new Poltergeist{ world,position });
	}
	else if (num == 5) {//SurogSakones
		world->add_actor(new SurogSakones{ world,position });
	}
	else if (num == -1) {

	}

}