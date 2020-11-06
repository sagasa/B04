#ifndef MAP_GENERATOR_H_
#define MAP_GENERATOR_H_

#include"Actor.h"
#include"CSVReader.h"

//敵生成クラス
class MapGenerator : public Actor {
public:
	//コンストラクタ
	MapGenerator(IWorld* world, const std::string& file_name);
	//更新
	virtual void update(float delta_time)override;
private:
	//敵の生成
	void generate(int row,int column);

private:
	//出現データ
	CsvReader csv_;
	//現在の読み込み位置(行)
	int current_row_{ 0 };
	//現在の読み込み位置(列)
	int current_column_{ 0 };
};


#endif