#ifndef MAP_GENERATOR_H_
#define MAP_GENERATOR_H_

#include"Actor.h"
#include"CSVReader.h"

//�G�����N���X
class MapGenerator : public Actor{
public:
	//�R���X�g���N�^
	MapGenerator(IWorld* world, const std::string& file_name);
	//�X�V
	void update(float delta_time) override;

private:
	//�G�̐���
	void generate(IWorld* world,const GSvector3&position);

private:
	//�o���f�[�^
	CsvReader csv_;
	//���݂̓ǂݍ��݈ʒu(�s)
	int current_row_{ 0 };
	//�v���C���[
	Actor* player_ = nullptr;
};


#endif