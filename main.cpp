#include <GSgame.h> // gslib::Game�ɕK�v
#include<iostream>

// �Q�[���N���X
class MyGame : public gslib::Game {
    // �J�n
    void start() override {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
        using namespace std;
        cout << "Vendor :" << glGetString(GL_VENDOR) << '\n';
        cout << "GPU : " << glGetString(GL_RENDERER) << '\n';
        cout << "OpenGL ver. " << glGetString(GL_VERSION) << '\n';
       // cout << "�y�g���@�\�ꗗ�z" << endl;
       // cout << glGetString(GL_EXTENSIONS) << endl;

        // ���b�V���̓ǂݍ���
        gsLoadMesh(0, "Assets/Model/vehicle_playerShip.msh");
    }
    // �X�V
    void update(float delta_time) override {
        ++i;
    }
    

    int i = 0;

    // �`��
    void draw() override {
        glPushMatrix();

        //
        // ���@�������v���X�����������悤�ɉ�]������
        glRotatef(i % 360, 0.0f, 1.0f, 0.0f);   // y���܂���180����]
        glRotatef(90, 1.0f, 0.0f, 0.0f);   // x���܂���-90����]
        // ���b�V���`��
        gsDrawMesh(0);
        glPopMatrix();
    }
    // �I��
    void end() override {
        // ���b�V���̍폜
        gsDeleteMesh(0);
    }
};

// ���C���֐�
int main() {
    return MyGame().run();
}