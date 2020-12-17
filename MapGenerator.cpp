#include"MapGenerator.h"
#include"IWorld.h"
#include"Player.h"
#include"CarGhost.h"
#include"RushGhost.h"
#include"Poltergeist.h"
#include"NormalGhost.h"
#include"SurogSakones.h"
#include"player_paladin.h"

const float size_{ 1 };

//コンストラクタ
MapGenerator::MapGenerator(IWorld* world, const std::string& file_name) {
	csv_.load(file_name);//CSVファイルの読み込み
	while (current_row_ < csv_.rows()-1) {
		current_row_++;
		generate(world);
		
	}
}

//生成
void MapGenerator::generate(IWorld* world) {
	
	//文字を取得
	std::string name = csv_.get(current_row_,0);
	//座標を取得
	GSvector3 position{
		csv_.getf(current_row_,size_ + 0),
		csv_.getf(current_row_,size_ + 1),
		csv_.getf(current_row_,size_ + 2)
	};
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
	}
	else if (name == "SurogSakones")
	{
		world->add_actor(new SurogSakones{ world,position });
	}

}