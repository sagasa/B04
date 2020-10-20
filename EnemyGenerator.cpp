#include"EnemyGenerator.h"
#include"IWorld.h"

const int CsvTime{ 0 };    //タイマーの列
const int CsvName{ 1 };    //名前の列
const int CsvPosition{ 2 };//座標の列
const int CsvVelocity{ 5 };//移動量の列

//コンストラクタ
EnemyGenerator::EnemyGenerator(IWorld* world, const std::string& file_name) {
	world_ = world;
	name_ = "EnemyGenerator";
	tag_ = "Generator";
	csv_.load(file_name);//CSVファイルの読み込み
}

//更新
void EnemyGenerator::update(float delta_time) {
	//CSVのデータがまだある && 出現時間であれば
	while (current_row_ < csv_.rows() && (csv_.getf(current_row_, CsvTime) * 60.0f) <= timer_) {
		//敵の生成
		generate();
		//CSVデータの読み込み行を更新
		++current_row_;
	}
	//出現タイマーの更新
	timer_ += delta_time;
}

//生成
void EnemyGenerator::generate() {
	//名前を取得
	std::string name = csv_.get(current_row_, CsvName);
	//座標を取得
	GSvector3 position{
		csv_.getf(current_row_,CsvPosition + 0),
		csv_.getf(current_row_,CsvPosition + 1),
		csv_.getf(current_row_,CsvPosition + 2) 
	};
	//キャラクタの生成
	if (name == "Enemy") {
		//敵の生成
	}
	
}