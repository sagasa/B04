#include"MapGenerator.h"
#include"IWorld.h"
#include"Player.h"
#include"CarGhost.h"
#include"RushGhost.h"
#include"Poltergeist.h"
#include"SurogSakones.h"

const int CsvTime{ 0 };    //�^�C�}�[�̗�
const int CsvName{ 1 };    //���O�̗�
const int CsvPosition{ 2 };//���W�̗�
const int CsvVelocity{ 5 };//�ړ��ʂ̗�

const float size_{ 1 };

//�R���X�g���N�^
MapGenerator::MapGenerator(IWorld* world, const std::string& file_name) {
	world_ = world;
	name_ = "EnemyGenerator";
	tag_ = "Generator";
	csv_.load(file_name);//CSV�t�@�C���̓ǂݍ���
}

//�X�V
void MapGenerator::update(float delta_time) {
	for (int column = 0;column < csv_.columns(); column++) {
		for (int row = 0;row < csv_.rows(); row++) {
			generate(row,column);
		}
	}
	//CSV�̃f�[�^���܂����� && �o�����Ԃł����
	while (current_row_ < csv_.rows()) {
		//�G�̐���
		//generate();
		//CSV�f�[�^�̓ǂݍ��ݍs���X�V
		++current_row_;
	}
	
}

//����
void MapGenerator::generate(int row,int column) {
	//���O���擾
	std::string name = csv_.get(row, column);
	//���W���擾
	GSvector3 position{
		column * size_,
		row * size_,
		0.0f
	};
	//�L�����N�^�̐���
	if (name == "Block") {
		//�u���b�N�̐���
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