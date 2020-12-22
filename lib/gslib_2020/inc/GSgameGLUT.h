#ifndef GS_GAME_GLUT_H_
#define GS_GAME_GLUT_H_

#include <string>
#include <gslib.h>
#include <sstream>

#pragma comment(linker, "/entry:mainCRTStartup")

// �Q�[���N���X
class Game {
public:
    int run();
protected:
    Game(int width = 640, int height = 480, bool fullScreen = false, float refreshRate = 60.0f);
    virtual ~Game() {}
    virtual void start() {}
    virtual void update(float frameTimer) {}
    virtual void draw() {}
    virtual void end() {}

private:
    void initilaizeGL(void);
    void setSwapInterval(int interval);
    static void activate(int state);
    static void display(void);
    static void idle(void);
    static void reshape(int width, int height);
    static void destroy(void);
    void wait();
    static GSdouble getTime();

private:
    std::string mWindowTitle;
    int         mWindowPositionX;
    int         mWindowPositionY;
    int         mWindowWidth;
    int         mWindowHeight;
    bool        mIsFullScreenMode;
    float       mPerspectiveFov;
    float       mPerspectiveNear;
    float       mPerspectiveFar;
    double      mPreviosTime;
    double      mFrameTime;
    double      mDeltaTime;

    static Game* mInstance;
};

inline Game::Game(int width, int height, bool fullScreen, float refreshRate) :
    mWindowTitle("GameWindow"),
    mWindowPositionX(0),
    mWindowPositionY(0),
    mWindowWidth(width),
    mWindowHeight(height),
    mIsFullScreenMode(fullScreen),
    mPerspectiveFov(45.0f),
    mPerspectiveNear(0.3f),
    mPerspectiveFar(1000.0f),
    mFrameTime(1.0 / refreshRate),
    mPreviosTime(0.0),
    mDeltaTime(0.0f) {
    // �������g�̃C���X�^���X��ݒ�
    mInstance = this;

    // �f�k�t�s�̏�����
    int argc = 1;
    char name[] = "game.exe";
    char* argv[] = { name };
    glutInit(&argc, argv);

    // �E�B���h�E�̐ݒ���s��
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowPosition(mWindowPositionX, mWindowPositionY);
    glutInitWindowSize(mWindowWidth, mWindowHeight);
    // �t���X�N���[�����[�h���H
    if (mIsFullScreenMode) {
        std::ostringstream mode;
        // �t���X�N���[�����[�h�ݒ蕶������쐬
        mode << mWindowWidth << "x" << mWindowHeight << "@" << (int)refreshRate;
        // �t���X�N���[�����[�h�ɐݒ�
        glutGameModeString(mode.str().c_str());
        glutEnterGameMode();
        // �t���X�N���[���̏ꍇ�̓}�E�X�J�[�\��������
        glutSetCursor(GLUT_CURSOR_NONE);
    } else {
        // �E�B���h�E���[�h
        glutCreateWindow(mWindowTitle.c_str());
    }
    // �n�������f�k�̏�����
    initilaizeGL();
    // �����̏�����
    gsRandamize();
    // �O���t�B�b�N�X�V�X�e���̏�����
    gsInitGraphics();
    // �T�E���h�V�X�e���̏�����
    gsInitSound((HWND)_glutGetHWND());
    // ���̓f�o�C�X�V�X�e���̏�����
    gsInitInput((HWND)_glutGetHWND());
    // �u����������L���ɂ���
    setSwapInterval(1);
}

inline int Game::run() {
    // �A�v���P�[�V�����̏���������
    start();
    // �I�������̊֐���ݒ肷��
    _glutDestroyFunc(destroy);
    // �E�B���h�E�T�C�Y�ύX���̊֐���ݒ�
    glutReshapeFunc(reshape);
    // �\�������̊֐���ݒ�
    glutDisplayFunc(display);
    // �C�x���g�̂Ȃ����̊֐���ݒ�
    glutIdleFunc(idle);
    // �E�B���h�E���A�N�e�B�u�ɂȂ������̊֐���ݒ�
    _glutActivateFunc(activate);
    // �t���[���^�C�}�̃��Z�b�g
    gsFrameTimerReset();
    // ���C�����[�v�������s��
    glutMainLoop();
    return 0;
}

inline void Game::initilaizeGL() {
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

inline void Game::destroy(void) {
    // �Q�[���I������
    mInstance->end();
    // ���̓f�o�C�X�V�X�e���̏I������
    gsFinishInput();
    // �T�E���h�V�X�e���̏I������
    gsFinishSound();
    // �O���t�B�b�N�X�V�X�e���̏I������
    gsFinishGraphics();
}

inline void Game::reshape(int width, int height) {
    // �������O�ɂȂ�Ȃ��悤�ɒ���
    height = (height == 0) ? 1 : height;
    // �r���[�|�[�g�̐ݒ�
    glViewport(0, 0, width, height);
    // �����ˉe�̕ϊ��s���ݒ肷��
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(
        mInstance->mPerspectiveFov,
        (float)width / (float)height,
        mInstance->mPerspectiveNear,
        mInstance->mPerspectiveFar
    );
    // ���f���r���[���[�h�ɂ���
    glMatrixMode(GL_MODELVIEW);
    // �E�B���h�E�̕��ƍ������X�V����
    mInstance->mWindowWidth = width;
    mInstance->mWindowHeight = height;
}

inline void Game::idle(void) {
    // ���̓f�o�C�X�̓ǂݍ���
    gsReadInput();
    // �Q�[���X�V����
    double deltaTime = mInstance->mDeltaTime * 60.0;
    deltaTime = (deltaTime > 2.0) ? 2.0 : deltaTime;
    mInstance->update((float)deltaTime);
    // �G�X�P�[�v�L�[�������ꂽ���H
    if (GetAsyncKeyState(VK_ESCAPE) != 0) {
        if (mInstance->mIsFullScreenMode) {
            // �Q�[�����[�h���I������
            glutLeaveGameMode();
        } else {
            // �I���������Ăяo��
            mInstance->destroy();
        }
        // �����I������
        std::exit(0);
    }
    // �`�揈�����Ăяo��
    glutPostRedisplay();
}

inline void Game::display(void) {
    // ��ʃN���A
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // �Q�[���`�揈��
    mInstance->draw();
    // �_�u���o�b�t�@�̐؂�ւ�
    glutSwapBuffers();
    // �^�C�}�E�F�C�g
    mInstance->wait();
}

inline void Game::activate(int state) {
    // ���̓f�o�C�X�V�X�e���ɃA�N�e�B�u��Ԃ�`����
    gsActivateInput(state);
    // �T�E���h�V�X�e���ɃA�N�e�B�u��Ԃ�`����
    gsActivateSound(state);
}

inline void Game::setSwapInterval(int interval) {
    // wglSwapIntervalEXT�g���֐��̃|�C���^�֐��^
    typedef BOOL(WINAPI * LPFNWGLSWAPINTERVALEXTPROC)(int interval);
    LPFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
    // SwapIntervalEXT�̊֐��|�C���^���擾
    wglSwapIntervalEXT = (LPFNWGLSWAPINTERVALEXTPROC)
        wglGetProcAddress("wglSwapIntervalEXT");
    // SwapIntervalEXT�֐����擾�ł������H
    if (wglSwapIntervalEXT != NULL) {
        // �X���b�v�̃C���^�[�o����ݒ�
        wglSwapIntervalEXT(interval);
    }
}

inline void Game::wait() {
    // �Œ���̃E�F�C�g
    while ((getTime() - mPreviosTime) < mFrameTime);
    // �f���^�^�C���̌v�Z
    mDeltaTime = getTime() - mPreviosTime;
    // �O��^�C�}��ۑ�
    mPreviosTime = getTime();
}

inline GSdouble Game::getTime() {
    LARGE_INTEGER time;
    QueryPerformanceCounter(&time);
    LARGE_INTEGER		freq;
    QueryPerformanceFrequency(&freq);
    return (GSdouble)time.QuadPart / (GSdouble)freq.QuadPart;
}

// �R�[���o�b�N�֐��̂��߂̐ÓI�����o�ϐ�
Game* Game::mInstance = 0;

#endif

