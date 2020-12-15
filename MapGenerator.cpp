#include"MapGenerator.h"
#include"IWorld.h"
#include"Player.h"
#include"CarGhost.h"
#include"RushGhost.h"
#include"Poltergeist.h"
#include"SurogSakones.h"

const float size_{ 1 };

//�R���X�g���N�^
MapGenerator::MapGenerator(IWorld* world, const std::string& file_name) {
	csv_.load(file_name);//CSV�t�@�C���̓ǂݍ���
	while (current_row_ < csv_.rows()) {
		current_row_++;
		generate();
		
	}
}

//����
void MapGenerator::generate() {
	
	//�������擾
	std::string name = csv_.get(current_row_, 0);
	//���W���擾
	GSvector3 position{
		csv_.getf(current_row_,size_ + 0),
		csv_.getf(current_row_,size_ + 1),
		csv_.getf(current_row_,size_ + 2)
	};
	

}