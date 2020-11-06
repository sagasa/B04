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

// ゲームクラス
class MyGame : public gslib::Game {
    
    // ワールドクラス
    World world_;

    GLuint vrtVBO = 0, idxVBO = 0;

	
    // 開始
    void start() override {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
        using namespace std;
        cout << "Vendor :" << glGetString(GL_VENDOR) << '\n';
        cout << "GPU : " << glGetString(GL_RENDERER) << '\n';
        cout << "OpenGL ver. " << glGetString(GL_VERSION) << '\n';
       // cout << "【拡張機能一覧】" << endl;
       // cout << glGetString(GL_EXTENSIONS) << endl;

          // 背景用画像の読み込み
        gsLoadTexture(Texture_BgTileNebulaGreen, "Assets/BG/tile_nebula_green_dff.png");
        // プレーヤー弾画像を読み込み
        gsLoadTexture(Texture_EffectLazerOrange, "Assets/Effect/fx_lazer_orange_dff.png");
        // 敵弾画像を読み込み
        gsLoadTexture(Texture_EffectLazerCyan, "Assets/Effect/fx_lazer_cyan_dff.png");
        // プレーヤメッシュの読み込み
        gsLoadMesh(Mesh_Player, "Assets/Model/vehicle_playerShip.msh");
        // CarGhostのメッシュの読み込み
        gsLoadMesh(Mesh_CarGhost, "Assets/Model/Ghost.msh");
        // RushGhostのメッシュの読み込み
        gsLoadMesh(Mesh_RushGhost, "Assets/Model/Ghost.msh");
        //Poltergeistのメッシュの読み込み
        gsLoadMesh(Mesh_Poltergeist, "Assets/Model/Ghost.msh");
        //SurogSakonesのメッシュの読み込み
        gsLoadMesh(Mesh_SurogSakones, "Assets/model/Enemy/Ghost_T-pose.msh");
        //SurogSakonesのスケルトンの読み込み
        gsLoadSkeleton(Skeleton_SurogSakones,"Assets/model/Enemy/Ghost_T-pose.skl");
        //SurogSakonesのアニメーションの読み込み
        gsLoadAnimation(Animation_SurogSakones, "Assets/model/Enemy/Ghost_T-pose.anm");

        // フィールドの追加
        world_.add_field(new Field{ Texture_BgTileNebulaGreen });
        // カメラの追加
        world_.add_camera(new Camera{ &world_ });
        // ライトの追加
        world_.add_light(new Light{ &world_ });
        // プレーヤの追加
        world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
        //エネミー1
        //world_.add_actor(new CarGhost{ &world_,GSvector3{0.0f,1.0f,0.0f} });
        //エネミー2
        //world_.add_actor(new RushGhost{ &world_,GSvector3{1.0f,0.0f,0.0f} });
        //ボス
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
    // 更新
    void update(float delta_time) override {
        world_.update(delta_time);
        ++i;
    }
    

    int i = 0;

   
    // 描画
    void draw() override {
        glPushMatrix();
        //*

        world_.draw();
    	//
        // 自機がｙ軸プラス方向を向くように回転させる
        glRotatef(i % 360, 0.0f, 1.0f, 0.0f);   // y軸まわりに180°回転
        glRotatef(90, 1.0f, 0.0f, 0.0f);   // x軸まわりに-90°回転
        // メッシュ描画
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
    // 終了
    void end() override {
        glDeleteBuffers(1, &vrtVBO);
        glDeleteBuffers(1, &idxVBO);
    	
        world_.clear();
        // メッシュの削除
        gsDeleteMesh(0);
    }

public:
    MyGame() : Game(1000, 1000) {

    }
};

// メイン関数
int main() {
    return MyGame().run();
}