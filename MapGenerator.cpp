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
	while (current_row_ < csv_.rows()) {
		current_row_++;
		generate();
		
	}
}

//生成
void MapGenerator::generate() {
	
	//文字を取得
	std::string name = csv_.get(current_row_, 0);
	//座標を取得
	GSvector3 position{
		csv_.getf(current_row_,size_ + 0),
		csv_.getf(current_row_,size_ + 1),
		csv_.getf(current_row_,size_ + 2)
	};
	

}