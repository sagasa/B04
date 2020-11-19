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
#include "DrawUtil.h"
#include <iostream>
#include "ByteBuf.h"
#include "MapObject.h"
#include "TestObj.h"
#include"SceneManager.h"
#include"EnemyTestScene.h"
#include"BossTestScene.h"
#include"TitleScene.h"
#include"PlayerTestScene.h"



void glError()
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "gl error : ";
        switch (err)
        {
        case GL_INVALID_ENUM:
            std::cout << "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            std::cout << "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            std::cout << "GL_INVALID_OPERATION";
            break;
        case GL_STACK_OVERFLOW:
            std::cout << "GL_STACK_OVERFLOW";
            break;
        case GL_STACK_UNDERFLOW:
            std::cout << "GL_STACK_UNDERFLOW";
            break;
        case GL_OUT_OF_MEMORY:
            std::cout << "GL_OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_CONTEXT_LOST:
            std::cout << "GL_CONTEXT_LOST";
            break;
        case GL_TABLE_TOO_LARGE:
            std::cout << "GL_TABLE_TOO_LARGE";
            break;
        }
        std::cout << std::endl;
    }
}

// �Q�[���N���X
class MyGame : public gslib::Game {
    
    // ���[���h�N���X
    World world_;
    SceneManager scene_;
    GLuint vrtVBO, idxVBO;
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
        //�X�J�C�{�b�N�X�̓ǂݍ���
        gsLoadMesh(Mesh_Skybox, "Assets/Skybox/skydome.msh");
        //�`��p�I�N�c���[�̓ǂݍ���
        gsLoadOctree(Octree_Stage, "Assets/Octree/stage.oct");
        //�Փ˔���p�I�N�c���[�̓ǂݍ���
        gsLoadOctree(Octree_Collider, "Assets/Octree/stage_collider.oct");


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
        gsLoadMesh(Mesh_RushGhost, "Assets/Model/Enemy/Ghost2.msh");
        //Poltergeist�̃��b�V���̓ǂݍ���
        gsLoadMesh(Mesh_Poltergeist, "Assets/Model/Enemy/Ghost2.msh");
        //�G�l�~�[�̃X�P���g���ƃA�j���[�V������ǉ�
        gsLoadSkeleton(Mesh_CarGhost,"Assets/Model/Enemy/Ghost.skl");
        gsLoadAnimation(Mesh_CarGhost, "Assets/Model/Enemy/Ghost.anm");
        //SurogSakones�̃��b�V���̓ǂݍ���
        gsLoadMesh(Mesh_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.msh");
        //SurogSakones�̃X�P���g���̓ǂݍ���
        gsLoadSkeleton(Mesh_SurogSakones,"Assets/Model/Enemy/Ghost_T-pose.skl");
        //SurogSakones�̃A�j���[�V�����̓ǂݍ���
        gsLoadAnimation(Mesh_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.anm");

        // �t�B�[���h�̒ǉ�
        world_.add_field(new Field{Octree_Stage,Octree_Collider,Mesh_Skybox});
        // �J�����̒ǉ�
        world_.add_camera(new Camera{ &world_ });
        // ���C�g�̒ǉ�
        world_.add_light(new Light{ &world_ });
        // �v���[���̒ǉ�
        world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 60.0f, 0.0f } });
        //�G�l�~�[1
        world_.add_actor(new CarGhost{ &world_,GSvector3{70.0f,30.0f,0.0f} });
        //�G�l�~�[2
        world_.add_actor(new RushGhost{ &world_,GSvector3{100.0f,0.0f,0.0f} });
        //�G�l�~�[3
        world_.add_actor(new Poltergeist{ &world_,GSvector3{70.0f,-50.0f,0.0f} });
        //�{�X
        world_.add_actor(new SurogSakones{ &world_,GSvector3{1.0f,1.0f,0.0f} });

        //�V�[���ǉ�
        scene_.add("EnemyTestScene", new EnemyTestScene());
        scene_.add("BossTestScene", new BossTestScene());
        scene_.add("PlayerTestScene", new PlayerTestScene());
        scene_.add("TitleScene", new TitleScene());
        scene_.change("TitleScene");
        glError();
    }

    
    // �X�V
    void update(float delta_time) override {
        world_.update(delta_time);

    	//gsCalculateRay()

        int x = 0;
        int y = 0;
        gsGetMouseCursorPosition(&x, &y);

        GSvector3 ray_pos, ray_vec;
        gsCalculateRay(x, y, &ray_pos.x, &ray_pos.y, &ray_pos.z, &ray_vec.x, &ray_vec.y, &ray_vec.z);
            
        using namespace std;
        //cout <<x << " " << y <<" -> "<<result.x <<" "<<result.y<<" "<<result.z<< " "<<vp[0] << " " << vp[1] <<endl;
        //cout <<x << " " << y <<" -> "<< ray_pos.x <<" "<< ray_pos.y<<" "<< ray_pos.z << " " << ray_vec.x << " " << ray_vec.y << " " << ray_vec.z <<endl;
        ++i;
        scene_.update(delta_time);
    }
    

    int i = 0;

   
    // �`��
    void draw() override {
        glPushMatrix();
        //world_.draw();
        gsDrawMesh(Mesh_Player);
        glPopMatrix();
    	
        glPushMatrix();
        using namespace collisions;
        using namespace std;

        scene_.draw();


        glPopMatrix();
        glError();
    }
    // �I��
    void end() override {
        world_.clear();
        // ���b�V���̍폜
        gsDeleteMesh(0);
        gsDeleteMesh(Mesh_Player);
        gsDeleteMesh(Mesh_CarGhost);
        gsDeleteMesh(Mesh_RushGhost);
        gsDeleteMesh(Mesh_Poltergeist);
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