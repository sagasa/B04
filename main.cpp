#include <GSgame.h>
#include "Assets.h"
#include "ActorManager.h"
#include "Player.h"
#include"CarGhost.h"
#include"RushGhost.h"
#include"SurogSakones.h"
#include "Light.h"
#include "Camera.h"
#include "Field.h"
#include "World.h"
#include "DrawUtil.h"
#include <iostream>
#include "ByteBuf.h"
#include "TestObj.h"


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

    GLuint vrtVBO = 0, idxVBO = 0;

	
    // �J�n
    void start() override {
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
        gsLoadMesh(Mesh_CarGhost, "Assets/Model/Ghost.msh");
        // RushGhost�̃��b�V���̓ǂݍ���
        gsLoadMesh(Mesh_RushGhost, "Assets/Model/Ghost.msh");
        //Poltergeist�̃��b�V���̓ǂݍ���
        gsLoadMesh(Mesh_Poltergeist, "Assets/Model/Ghost.msh");
        //SurogSakones�̃��b�V���̓ǂݍ���
        gsLoadMesh(Mesh_SurogSakones, "Assets/model/Enemy/Ghost_T-pose.msh");
        //SurogSakones�̃X�P���g���̓ǂݍ���
        gsLoadSkeleton(Skeleton_SurogSakones,"Assets/model/Enemy/Ghost_T-pose.skl");
        //SurogSakones�̃A�j���[�V�����̓ǂݍ���
        gsLoadAnimation(Animation_SurogSakones, "Assets/model/Enemy/Ghost_T-pose.anm");

        // �t�B�[���h�̒ǉ�
        world_.add_field(new Field{ Texture_BgTileNebulaGreen });
        // �J�����̒ǉ�
        world_.add_camera(new Camera{ &world_ });
        // ���C�g�̒ǉ�
        world_.add_light(new Light{ &world_ });
        // �v���[���̒ǉ�
        world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
        //�G�l�~�[1
        //world_.add_actor(new CarGhost{ &world_,GSvector3{0.0f,1.0f,0.0f} });
        //�G�l�~�[2
        //world_.add_actor(new RushGhost{ &world_,GSvector3{1.0f,0.0f,0.0f} });
        //�{�X
        //world_.add_actor(new SurogSakones{ &world_,GSvector3{1.0f,1.0f,0.0f} });

        for (int i = 0; i < 2; ++i)
        {
        	
            world_.add_actor(new TestObj{ &world_,GSvector3{1.0f,1.0f,0.0f} });
	        
        }
       

        glError();

        GLfloat vertices[] = { 0, 0, 0,  100, 0, 0,  100, 100, 0,  0, 100, 0 };

        int indices[] = { 0, 1, 2,  0, 2, 3 };
        
        glGenBuffers(1, &vrtVBO);
        glBindBuffer(GL_ARRAY_BUFFER, vrtVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &idxVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);


        //ByteBuf buf{ 1024 };
        //cout << "Vendor :" << buf.size() << " " << buf.data() << '\n';

    	glError();
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
        //*

        world_.draw();
    	//
        // ���@�������v���X�����������悤�ɉ�]������
        glRotatef(i % 360, 0.0f, 1.0f, 0.0f);   // y���܂���180����]
        glRotatef(90, 1.0f, 0.0f, 0.0f);   // x���܂���-90����]
        // ���b�V���`��
        //*/
    	
        gsDrawMesh(Mesh_Player);
        glPopMatrix();
        glPushMatrix();
        using namespace collisions;
        using namespace std;

        Box2D box{ Vec2{0,0}, Vec2{2,2},0 };
        box.p = Vec2{ 4,0 };
        //cout << box.isInside(Vec2{0,0}) <<" "<< box.isInside(Vec2{ 4,0 }) <<endl;

    	/*
        glBindBuffer(GL_ARRAY_BUFFER, vrtVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_DOUBLE, 0, 0);
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glDisableClientState(GL_VERTEX_ARRAY);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        //*/
    

        //drawUtil::drawLine(Vec2(), Vec2(100,100));
        //*/

        glPopMatrix();
        glError();
    }
    // �I��
    void end() override {
        glDeleteBuffers(1, &vrtVBO);
        glDeleteBuffers(1, &idxVBO);
    	
        world_.clear();
        // ���b�V���̍폜
        gsDeleteMesh(0);
    }

public:
    MyGame() : Game(1000, 1000) {

    }
};

// ���C���֐�
int main() {
    return MyGame().run();
}