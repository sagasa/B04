#include"SceneManager.h"

#include <iostream>

#include"SceneNull.h"

static SceneNull scene_null;

//�R���X�g���N�^
SceneManager::SceneManager() :
	current_scene_{ &scene_null } {
	load_end_ = [this] {
		//���̃V�[�����J�n �߂�܂ŃA�b�v�f�[�g���Ȃ��悤��
		next_scene_->start();
		current_scene_ = next_scene_;
		next_scene_ = nullptr;
	};
}

//�f�X�g���N�^
SceneManager::~SceneManager() {
	clear();
}
//�X�V
void SceneManager::update(float delta_time) {
	//�V�[���̍X�V
	current_scene_->update(delta_time);
	//���[�h���I����Ă����
	if (next_scene_!=nullptr&&loader_.count_ == 0)
	{
		load_end_();
	}

	//�V�[�����I�����Ă��邩�H
	if (current_scene_->is_end()) {
		//�V�[����ύX����
		change(current_scene_->next());
	}
}

//�`��
void SceneManager::draw() const {
	//���݂̃V�[����`��
	current_scene_->draw();
}

//�I��
void SceneManager::end() {
	//���݂̃V�[�����I��
	current_scene_->end();
	//null�V�[���ɂ��Ă���
	current_scene_ = &scene_null;
}

//�V�[���̒ǉ�
void SceneManager::add(const std::string& name, IScene* scene) {
	scenes_[name] = scene;
}

//�V�[���̕ύX
void SceneManager::change(const std::string& name) {
	//���݂̃V�[�����I��
	end();
	//���݂̃V�[����ύX
	next_scene_ = scenes_[name];
	next_scene_->load(loader_);
	//�Y��ɏ������������ł�
	if(loader_.count_==0)
	{
		load_end_();
	}
	//loader_.on_end(load_end_);
}



//�V�[���̏���
void SceneManager::clear() {
	end();
	for (auto & pair : scenes_) {
		delete pair.second;
	}
	scenes_.clear();
}