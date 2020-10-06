#ifndef GSLIB_GS_GAME_H_
#define GSLIB_GS_GAME_H_

#include <gslib.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <functional>
#include <string>
#include <stdexcept>

#pragma comment(linker, "/entry:mainCRTStartup")

namespace gslib {

// GSLIB�E�B���h�E�N���X
class Window {
public:
	// �R���X�g���N�^
	Window(int width, int height, const std::string& name, bool fullScreen, float refreshRate) : window_(NULL) {
		if (!glfwInit()) {
			throw std::runtime_error("error glfwInit");
		}
//		glfwWindowHint(GLFW_SAMPLES, 4);
		auto monitor = fullScreen ? glfwGetPrimaryMonitor() : NULL;
        glfwWindowHint(GLFW_REFRESH_RATE, refreshRate);
		window_ = glfwCreateWindow(width, height, name.c_str(), monitor, NULL);
		if (!window_)  {
			glfwTerminate();
			throw std::runtime_error("error glfwCreateWindow");
		}
        // ���݂̃��j�^�[���擾
        monitor_ = glfwGetWindowMonitor(window_);
        if (monitor_ == NULL) {
            monitor_ = glfwGetPrimaryMonitor();
        }
        // ���݂̃��t���b�V�����[�g��ۑ�
        const  GLFWvidmode * mode = glfwGetVideoMode(monitor_);
        refreshRate_ = mode->refreshRate;
		glfwMakeContextCurrent(window_);
		glfwSetWindowUserPointer(window_, this);
	}
	// �f�X�g���N�^
	~Window() {
		glfwTerminate();
	}
	// �E�B���h�E��������H
	bool isClose() const {
		return glfwWindowShouldClose(window_) != 0;
	}
	// �_�u���o�b�t�@�̌���
	void swapBuffers() {
		glfwSwapBuffers(window_);
	}
	// �C�x���g�̃|�[�����O
	void pollEvents() {
		glfwPollEvents();
	}
	// �X���b�v�C���^�[�o���̐ݒ�
	void swapInterval(int interval) {
		glfwSwapInterval(interval);
	}
	// �t�H�[�J�X���̃R�[���o�b�N��ݒ�
	void registerForcusCallback(std::function<void(bool)> fn) {
		forcusCallback_ = fn;
		glfwSetWindowFocusCallback(window_, &forcusCallback);
	}
	// ���T�C�Y���̃R�[���o�b�N��ݒ�
	void registerResizeCallback(std::function<void(int, int)> fn) {
		resizeCallback_ = fn;
		glfwSetWindowSizeCallback(window_, &resizeCallback);
	}
	// �E�B���h�E�n���h���̎擾
	HWND getHWND() const {
		return glfwGetWin32Window(window_);
	}
	// �^�C�}�̎擾
	double getTime() const {
		return glfwGetTime();
	}
    // ���t���b�V�����[�g�̎擾
	double getRefreshRate() const {
        return refreshRate_;
    }

private:
	static void forcusCallback(GLFWwindow* window, int focused) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->forcusCallback_(focused == GL_TRUE);
	}
	static void resizeCallback(GLFWwindow* window, int width, int height) {
		Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		self->resizeCallback_(width, height);
	}
private:
	// �R�s�[�֎~
	Window(const Window& other);
	Window& operator = (const Window& other);

private:
	// GLFW�̃E�B���h�E
	GLFWwindow* window_;
    // ���݂̃��j�^�[
    GLFWmonitor* monitor_;
	// �A�N�e�B�u�ɂȂ������̏���
	std::function<void(bool)> forcusCallback_;
	// �T�C�Y�ύX���̏���
	std::function<void(int, int)> resizeCallback_;
	// �N���[�Y���̏���
	std::function<void(void)> closeCallback_;
    // ���ۂ̃��t���b�V�����[�g
    double refreshRate_;
};

// GSLIB�f�o�C�X�N���X
class Device {
public:
	// �R���X�g���N�^
	Device(int width, int height, bool fullScreen, float refreshRate) :
		window_(width, height, "Game Window", fullScreen, refreshRate),
		frameTime_(1.0 / window_.getRefreshRate()),
		previosTime_(window_.getTime()),
		deltaTime_(0.0) {
		// �E�B���h�E�T�C�Y�ύX
		resize(width, height);
		// �E�B���h�E�T�C�Y�ύX���̊֐���ݒ�
		window_.registerResizeCallback([=](int w, int h) { resize(w, h); });
		// �E�B���h�E�A�N�e�B�u���̊֐���ݒ�
		window_.registerForcusCallback([=](bool flag) { activate(flag); });
		// �u����������L���ɂ���
		window_.swapInterval(1);
		// �n�������f�k�̏�����
		initilaizeOpneGL();
		// �����̏�����
		gsRandamize();
		// �T�E���h�V�X�e���̏�����
		gsInitSound(window_.getHWND());
		// ���̓f�o�C�X�V�X�e���̏�����
		gsInitInput(window_.getHWND());
		// �O���t�B�b�N�X�V�X�e���̏�����
		gsInitGraphics();
	}
	// �f�X�g���N�^
	~Device() {
		// �O���t�B�b�N�X�V�X�e���̏I������
		gsFinishGraphics();
		// ���̓f�o�C�X�V�X�e���̏I������
		gsFinishInput();
		// �T�E���h�V�X�e���̏I������
		gsFinishSound();
	}
	// �o�b�t�@���X���b�v
	void swap() {
		window_.swapBuffers();
		window_.pollEvents();
		gsReadInput();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// �Œ᎞�Ԃ̃E�F�C�g
		while ((window_.getTime() - previosTime_) < frameTime_) ;
		deltaTime_ = window_.getTime() - previosTime_;
		previosTime_ = window_.getTime();
	}
	// �N���[�Y���邩�H
	bool isClose() {
		return window_.isClose();
	}
    // ���t���b�V�����[�g�̎擾
	double getRefreshRate() const {
        return window_.getRefreshRate();
    }
	// �f���^�^�C���̎擾
	double getDeltaTime() const {
		return deltaTime_;
	}

private:
	void initilaizeOpneGL() {
		// �ʃJ�����O�̏�����L���ɂ���
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		// �f�v�X�o�b�t�@��1.0�ŃN���A����
		glClearDepth(1.0);
		// �e�v�X�e�X�g��L���ɂ���
		glEnable(GL_DEPTH_TEST);
		// �p�[�X�y�N�e�B�u�␳���s��
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		// �u�����h��L��
		glEnable(GL_BLEND);
		// �f�t�H���g�̃u�����h����ݒ�
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// �A���t�@�e�X�g��L��
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0);
		// �X�y�L�����̌v�Z�𕪗�
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		// �f�t�H���g�̎��_�ϊ��̕ϊ��s���ݒ肷��
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(
			0.0f, 0.0f, 40.0f,
			0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		);
		// �f�t�H���g�̃��C�g�̐ݒ�
		static const float lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		static const float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		static const float lightSpeclar[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		static const float lightPosition[] = { 100.0f, 100.0f, 100.0f, 0.0f };
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpeclar);
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		// ��ʏ���
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void activate(bool state) {
		// ���̓f�o�C�X�V�X�e���ɃA�N�e�B�u��Ԃ�`����
		gsActivateInput(state ? GS_TRUE : GS_FALSE);
		// �T�E���h�V�X�e���ɃA�N�e�B�u��Ԃ�`����
		gsActivateSound(state ? GS_TRUE : GS_FALSE);
	}
	void resize(int width, int height) {
		// �������O�ɂȂ�Ȃ��悤�ɒ���
		height = (height == 0) ? 1 : height;
		// �r���[�|�[�g�̐ݒ�
		glViewport(0, 0, width, height);
		// �����ˉe�̕ϊ��s���ݒ肷��
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(
			45.0f,
			(float)width / (float)height,
			0.3f,
			1000.0f
		);
		// ���f���r���[���[�h�ɂ���
		glMatrixMode(GL_MODELVIEW);
	}
private:
	// �R�s�[�֎~
	Device(const Device& other);
	Device& operator = (const Device& other);
private:
	// �E�B���h�E�N���X
	Window window_;
	// �t���[���^�C�}
	double frameTime_;
	// �O�t���[���̎���
	double previosTime_;
	// �f���^�^�C��
	double deltaTime_;
};

// �Q�[���N���X
class Game {
protected:
	// �R���X�g���N�^
	Game(int width = 640, int height = 480, bool fullScreen = false, float refreshRate = 60.0f) :
		width_(width),
		height_(height),
		fullScreen_(fullScreen),
        refreshRate_(refreshRate),
		device_(width, height, fullScreen, refreshRate) {
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);// ���������[�N���o
	}

public:
	// ���s
	int run() {
		start();
		while (!device_.isClose() && isRunning()) {
			float deltaTime = device_.getDeltaTime() / (1.0 / 60.0);
			deltaTime = (deltaTime > 2.0) ? 2.0 : deltaTime;
			update((float)deltaTime);
			draw();
			device_.swap();
		}
		end();
		return 0;
	}

private:
	// �J�n
	virtual void start() {}
	// �X�V
	virtual void update(float time) { (void)time; }
	// �`��
	virtual void draw() {}
	// �I��
	virtual void end() {}
	// ���s����
	virtual bool isRunning() { return GetAsyncKeyState(VK_ESCAPE) == 0; }

private:
	// �R�s�[�֎~
	Game(const Game& other);
	Game& operator = (const Game& other);

private:
	// �E�B���h�E�̕�
	int width_;
	// �E�B���h�E�̍���
	int height_;
	// �t���X�N���[�����H
	bool fullScreen_;
	// FPS
	float refreshRate_;
	// GSLIB�f�o�C�X�N���X
	Device device_;
};

} // namespace gslib

#endif
