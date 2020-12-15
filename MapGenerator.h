#ifndef MAP_GENERATOR_H_
#define MAP_GENERATOR_H_

#include"Actor.h"
#include"CSVReader.h"

//�G�����N���X
class MapGenerator{
public:
	//�R���X�g���N�^
	MapGenerator(IWorld* world, const std::string& file_name);
private:
	//�G�̐���
	void generate();

private:
	//�o���f�[�^
	CsvReader csv_;
	//���݂̓ǂݍ��݈ʒu(�s)
	int current_row_{ 0 };
	//���݂̓ǂݍ��݈ʒu(��)
	int current_column_{ 0 };
};


#endif