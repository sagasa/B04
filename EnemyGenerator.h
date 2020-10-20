#ifndef ENEMY_GENERATOR_H_
#define ENEMY_GENERATOR_H_

#include"Actor.h"
#include"CSVReader.h"

//敵生成クラス
class EnemyGenerator : public Actor {
public:
	//コンストラクタ
	EnemyGenerator(IWorld* world, const std::string& file_name);
	//更新
	virtual void update(float delta_time)override;
private:
	//敵の生成
	void generate();

private:
	//出現データ
	CsvReader csv_;
	//現在の読み込み位置
	int current_row_{ 0 };
	//出現タイマー
	float timer_{ 0.0f };
};


#endif

