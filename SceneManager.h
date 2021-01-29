#ifndef SCENE_MANAGER_H_
#define SCENE_MANAGER_H_

#include<unordered_map>
#include<string>

#include "resource_loader.h"

class IScene;
//�V�[���Ǘ��N���X
class SceneManager {
public:
	//�R���X�g���N�^
	SceneManager();
	//�f�X�g���N�^
	~SceneManager();
	//�X�V
	void update(float delta_time);
	//�`��
	void draw() const;
	//�I��
	void end();
	//�V�[���̒ǉ�
	void add(const std::string& name, IScene* scene);
	//�V�[���̕ύX
	void change(const std::string& name);
	//�V�[���̏���
	void clear();
	//�R�s�[�֎~
	SceneManager(const SceneManager& other) = delete;
	SceneManager& operator = (const SceneManager& other) = delete;
private:
	//�ǂݍ��݊��� ���̃V�[�������s
	std::function<void()> load_end_;
	//�V�[��
	std::unordered_map<std::string, IScene* > scenes_;
	resource_loader loader_;
	IScene* next_scene_;
	IScene* current_scene_;
	//���݂̃X�e�[�W�ԍ�
	int stage_number_ = 1;
};

#endif
