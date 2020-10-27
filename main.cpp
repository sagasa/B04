#include <GSgame.h>
#include "Assets.h"
#include "ActorManager.h"
#include "Player.h"
#include"CarGhost.h"
#include"RushGhost.h"
#include"Poltergeist.h"
#include"SurogSakones.h"
#include "Light.h"
#include "Camera.h"
#include "Field.h"
#include "World.h"
#include <iostream>

// �Q�[���N���X
class MyGame : public gslib::Game {
    // ���[���h�N���X
    World world_;

    // �J�n
    void start() override {
        //glfwSetWindowSize(,1920,1080);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
        using namespace std;
        cout << "Vendor :" << glGetString(GL_VENDOR) << '\n';
        cout << "GPU : " << glGetString(GL_RENDERER) << '\n';
        cout << "OpenGL ver. " << glGetString(GL_VERSION) << '\n';
       // cout << "�y�g���@�\�ꗗ�z" << endl;
       // cout << glGetString(GL_EXTENSIONS) << endl;

          // �w�i�p�摜�̓ǂݍ���
        gsLoadTexture(Texture_BgTileNebulaGreen, "Assets/BG/tile_nebula_green_dff.png");
        // �v���[���[�e�摜��ǂݍ���
        gsLoadTexture(Texture_EffectLazerOrange, "Assets/Effect/fx_lazer_orange_dff.png");
        // �G�e�摜��ǂݍ���
        gsLoadTexture(Texture_EffectLazerCyan, "Assets/Effect/fx_lazer_cyan_dff.png");
        // �v���[�����b�V���̓ǂݍ���
        gsLoadMesh(Mesh_Player, "Assets/Model/vehicle_playerShip.msh");
        // CarGhost�̃��b�V���̓ǂݍ���
        gsLoadMesh(Mesh_CarGhost, "Assets/Model/Enemy/Ghost.msh");
        // RushGhost�̃��b�V���̓ǂݍ���
        //gsLoadMesh(Mesh_RushGhost, "Assets/Model/Enemy/Ghost.msh");
        //Poltergeist�̃��b�V���̓ǂݍ���
        //gsLoadMesh(Mesh_Poltergeist, "Assets/Model/Enemy/Ghost.msh");
        //�G�l�~�[�̃X�P���g���ƃA�j���[�V������ǉ�
        gsLoadSkeleton(Skeleton_CarGhost,"Assets/Model/Enemy/Ghost.skl");
        gsLoadAnimation(Animation_CarGhost, "Assets/Model/Enemy/Ghost.anm");
        //SurogSakones�̃��b�V���̓ǂݍ���
        gsLoadMesh(Mesh_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.msh");
        //SurogSakones�̃X�P���g���̓ǂݍ���
        gsLoadSkeleton(Skeleton_SurogSakones,"Assets/Model/Enemy/Ghost_T-pose.skl");
        //SurogSakones�̃A�j���[�V�����̓ǂݍ���
        gsLoadAnimation(Animation_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.anm");

        // �t�B�[���h�̒ǉ�
        world_.add_field(new Field{ Texture_BgTileNebulaGreen });
        // �J�����̒ǉ�
        world_.add_camera(new Camera{ &world_ });
        // ���C�g�̒ǉ�
        world_.add_light(new Light{ &world_ });
        // �v���[���̒ǉ�
        world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
        //�G�l�~�[1
        world_.add_actor(new CarGhost{ &world_,GSvector3{0.0f,1.0f,0.0f} });
        //�G�l�~�[2
        world_.add_actor(new RushGhost{ &world_,GSvector3{1.0f,0.0f,0.0f} });
        //�G�l�~�[3
        world_.add_actor(new Poltergeist{ &world_,GSvector3{0.0f,0.0f,10.0f} });
        //�{�X
        world_.add_actor(new SurogSakones{ &world_,GSvector3{1.0f,1.0f,0.0f} });

    }
    // �X�V
    void update(float delta_time) override {
        world_.update(delta_time);
        ++i;
    }
    

    int i = 0;

    // �`��
    void draw() override {
        glPushMatrix();
        world_.draw();
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
        world_.clear();
        // ���b�V���̍폜
        gsDeleteMesh(0);
        gsDeleteMesh(Mesh_Player);
        gsDeleteMesh(Mesh_CarGhost);
        //gsDeleteMesh(Mesh_RushGhost);
        //gsDeleteMesh(Mesh_Poltergeist);
        gsDeleteMesh(Mesh_SurogSakones);
    }
public:
    MyGame() : Game(1920, 1080, false, 60) {

    }
};

// ���C���֐�
int main() {
    return MyGame().run();
}