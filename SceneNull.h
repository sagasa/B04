#ifndef SCENE_NULL_H_
#define SCENE_NULL_H_

#include"IScene.h"

//�k���V�[��
class SceneNull : public IScene {
public:
	//�J�n
	void start(int number) override {}
	//�X�V
	void update(float delta_time)override {}
	//�`��
	void draw() const override {}
	//�I�����Ă��邩�H
	bool is_end() const override {
		return false;
	}
	//���̃V�[����Ԃ�
	std::string next() const override {
		return "SceneNull";
	}
	//���݂̃X�e�[�W�ԍ���Ԃ�
	virtual int stage_number() const override {
		return 1;
	}
	//�I��
	void end() override {}
};

#endif;