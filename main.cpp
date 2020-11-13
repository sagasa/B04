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
        world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 60.0f, 0.0f } });
        //�G�l�~�[1
        world_.add_actor(new CarGhost{ &world_,GSvector3{70.0f,30.0f,0.0f} });
        //�G�l�~�[2
        world_.add_actor(new RushGhost{ &world_,GSvector3{100.0f,0.0f,0.0f} });
        //�G�l�~�[3
        world_.add_actor(new Poltergeist{ &world_,GSvector3{70.0f,-50.0f,0.0f} });
        //�{�X
        //world_.add_actor(new SurogSakones{ &world_,GSvector3{1.0f,1.0f,0.0f} });

        world_.add_actor(new MapObject{ &world_,GSvector3{0.0f,0.0f,0.0f}});

        for (int i = 0; i < 10; ++i)
        {
        	
           // world_.add_actor(new TestObj{ &world_,GSvector3{-40+4.0f*i,10.0f,0.0f} ,i});
	        
        }
       

        glError();

        //GLfloat vertices[] = { 0, 0, 0,  100, 0, 0,  100, 100, 0,  0, 100, 0 };

        //int indices[] = { 0, 1, 2,  0, 2, 3 };
        //glGenBuffers(1, &vrtVBO);
        //glBindBuffer(GL_ARRAY_BUFFER, vrtVBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

        //glGenBuffers(1, &idxVBO);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxVBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);


        //ByteBuf buf{ 1024 };
        //cout << "Vendor :" << buf.size() << " " << buf.data() << '\n';

    	glError();
    }

    
    // �X�V
    void update(float delta_time) override {
        world_.update(delta_time);

    	//gsCalculateRay()

        int x = 0;
        int y = 0;
        gsGetMouseCursorPosition(&x, &y);

        GSmatrix4	matProj;
        GSmatrix4	matView;
        GSmatrix4   matScreen;
        int vp[4];
        GSvector3   result;

        // �r���[�|�[�g�̎擾
        glGetIntegerv(GL_VIEWPORT, vp);
        // �X�N���[�����W�ϊ��s����쐬
        float w = vp[2] / 2.0f;
        float h = vp[3] / 2.0f;
        matScreen._11 = w; matScreen._22 = -h;
        matScreen._41 = w; matScreen._42 = h;

        // �����ϊ��s��̎擾
        glGetFloatv(GL_PROJECTION_MATRIX, (GLfloat*)&matProj);
        // ���_�ϊ��s��̎擾
        glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat*)&matView);
        // �t�s���
        matView.inverse();
        matProj.inverse();
        matScreen.inverse();
        // ���W�ϊ����s��
        GSvector3 cursor = GSvector3{ (float)x - w,vp[3] - (float)y - h,0 };


        GSmatrix4 tmp = matScreen * matProj * matView;
        gsVector3TransformCoord(&result, &cursor, &tmp);

        if (i%6==0&&gsGetMouseButtonState(GMOUSE_BUTTON_1))
        {
            if (gsGetKeyState(GKEY_W))
                world_.add_actor(new TestObj{ &world_,result.xy ,GSvector2{1,1},i });
            else if (gsGetKeyState(GKEY_D))
                world_.add_actor(new TestObj{ &world_,result.xy ,GSvector2{1,-1},i });
            else if (gsGetKeyState(GKEY_S))
                world_.add_actor(new TestObj{ &world_,result.xy ,GSvector2{-1,-1},i });
            else if (gsGetKeyState(GKEY_A))
                world_.add_actor(new TestObj{ &world_,result.xy ,GSvector2{-1,1},i });
            else
            {
                world_.add_actor(new TestObj{ &world_,result.xy ,GSvector2{1,1},i });
                world_.add_actor(new TestObj{ &world_,result.xy ,GSvector2{1,-1},i });
                world_.add_actor(new TestObj{ &world_,result.xy ,GSvector2{-1,-1},i });
                world_.add_actor(new TestObj{ &world_,result.xy ,GSvector2{-1,1},i });
            }
        }
        GSvector3 ray_pos, ray_vec;
        gsCalculateRay(x, y, &ray_pos.x, &ray_pos.y, &ray_pos.z, &ray_vec.x, &ray_vec.y, &ray_vec.z);
            
        using namespace std;
        //cout <<x << " " << y <<" -> "<<result.x <<" "<<result.y<<" "<<result.z<< " "<<vp[0] << " " << vp[1] <<endl;
        //cout <<x << " " << y <<" -> "<< ray_pos.x <<" "<< ray_pos.y<<" "<< ray_pos.z << " " << ray_vec.x << " " << ray_vec.y << " " << ray_vec.z <<endl;
        ++i;
    }
    

    int i = 0;

   
    // �`��
    void draw() override {
        glPushMatrix();
        world_.draw();
        gsDrawMesh(Mesh_Player);
        glPopMatrix();
    	
        glPushMatrix();
        using namespace collisions;
        using namespace std;

        
        //GSmatrix4 mat4;
    //glGetFloatv(GL_MODELVIEW,mat4.v);

    //GSvector3 pos = transform_.position();
    //std::cout << " pos [" << pos.x << ", "<<pos.y<<", "<<pos.z<<"]";
    //pos = mat4* pos;
    //std::cout << " : [" << pos.x << ", " << pos.y << ", " << pos.z << "]";

        
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
        //glDeleteBuffers(1, &vrtVBO);
        //glDeleteBuffers(1, &idxVBO);
    	
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