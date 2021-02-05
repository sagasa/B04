#include"MapGenerator.h"
#include"IWorld.h"
#include"Player.h"
#include"CarGhost.h"
#include"RushGhost.h"
#include"Poltergeist.h"
#include"NormalGhost.h"
#include"SurogSakones.h"
#include"DeathSmoke.h"
#include"player_paladin.h"

const float size_{ 1 };

//�R���X�g���N�^
MapGenerator::MapGenerator(IWorld* world, const std::string& file_name) {
	csv_.load(file_name);//CSV�t�@�C���̓ǂݍ���
	//���[���h�̐ݒ�
	world_ = world;
	//���O�̐ݒ�
	name_ = "MapGenerator";
	tag_ = "generatorTag";
	enable_collider_ = false;
	while (current_row_ < csv_.rows() - 1) {
		current_row_++;
		//���W���擾
		GSvector3 position{
			csv_.getf(current_row_,size_ + 0),
			csv_.getf(current_row_,size_ + 1),
			csv_.getf(current_row_,size_ + 2)
		};
		generate(world_, position);
	}
	current_row_++;
}

//�X�V
void MapGenerator::update(float delta_time) {

	//if (current_row_ < csv_.rows()) {
	//	//�v���C���[������
	//	player_ = world_->find_actor("Player");
	//	if (player_ == nullptr) {
	//		player_ = world_->find_actor("PlayerPaladin");
	//		if (player_ == nullptr) return;
	//	}

	//	//���W���擾
	//	GSvector3 position{
	//		csv_.getf(current_row_,size_ + 0),
	//		csv_.getf(current_row_,size_ + 1),
	//		csv_.getf(current_row_,size_ + 2)
	//	};

	//	if (player_->transform().position().x + 20.0f >= position.x) {
	//		generate(world_, position);
	//		current_row_++;
	//	}
	//}
	
}

//����
void MapGenerator::generate(IWorld* world,const GSvector3& position) {
	
	//�������擾
	std::string name = csv_.get(current_row_,0);

	//��������
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
		world->add_actor(new player_paladin{ world,position });
	}
	else if (name == "SurogSakones")
	{
		world->add_actor(new SurogSakones{ world,position });
	}
	else if (name == "DeathSmoke")
	{
		world->add_actor(new DeathSmoke{ world,position });
	}
}