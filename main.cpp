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

// ゲームクラス
class MyGame : public gslib::Game {
    
    // ワールドクラス
    World world_;
    SceneManager scene_;
    GLuint vrtVBO, idxVBO;
    // 開始
    void start() override {
        //glfwSetWindowSize(,1920,1080);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
        using namespace std;
        cout << "Vendor :" << glGetString(GL_VENDOR) << '\n';
        cout << "GPU : " << glGetString(GL_RENDERER) << '\n';
        cout << "OpenGL ver. " << glGetString(GL_VERSION) << '\n';
       // cout << "【拡張機能一覧】" << endl;
       // cout << glGetString(GL_EXTENSIONS) << endl;
        //スカイボックスの読み込み
        gsLoadMesh(Mesh_Skybox, "Assets/Skybox/skydome.msh");
        //描画用オクツリーの読み込み
        gsLoadOctree(Octree_Stage, "Assets/Octree/stage.oct");
        //衝突判定用オクツリーの読み込み
        gsLoadOctree(Octree_Collider, "Assets/Octree/stage_collider.oct");


          // 背景用画像の読み込み
        gsLoadTexture(Texture_BgTileNebulaGreen, "Assets/BG/tile_nebula_green_dff.png");
        // プレーヤー弾画像を読み込み
        gsLoadTexture(Texture_EffectLazerOrange, "Assets/Effect/fx_lazer_orange_dff.png");
        // 敵弾画像を読み込み
        gsLoadTexture(Texture_EffectLazerCyan, "Assets/Effect/fx_lazer_cyan_dff.png");
        // プレーヤメッシュの読み込み
        gsLoadMesh(Mesh_Player, "Assets/Model/vehicle_playerShip.msh");
        // CarGhostのメッシュの読み込み
        gsLoadMesh(Mesh_CarGhost, "Assets/Model/Enemy/Ghost.msh");
        // RushGhostのメッシュの読み込み
        gsLoadMesh(Mesh_RushGhost, "Assets/Model/Enemy/Ghost2.msh");
        //Poltergeistのメッシュの読み込み
        gsLoadMesh(Mesh_Poltergeist, "Assets/Model/Enemy/Ghost2.msh");
        //エネミーのスケルトンとアニメーションを追加
        gsLoadSkeleton(Mesh_CarGhost,"Assets/Model/Enemy/Ghost.skl");
        gsLoadAnimation(Mesh_CarGhost, "Assets/Model/Enemy/Ghost.anm");
        //SurogSakonesのメッシュの読み込み
        gsLoadMesh(Mesh_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.msh");
        //SurogSakonesのスケルトンの読み込み
        gsLoadSkeleton(Mesh_SurogSakones,"Assets/Model/Enemy/Ghost_T-pose.skl");
        //SurogSakonesのアニメーションの読み込み
        gsLoadAnimation(Mesh_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.anm");

        // フィールドの追加
        world_.add_field(new Field{Octree_Stage,Octree_Collider,Mesh_Skybox});
        // カメラの追加
        world_.add_camera(new Camera{ &world_ });
        // ライトの追加
        world_.add_light(new Light{ &world_ });
        // プレーヤの追加
        world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 60.0f, 0.0f } });
        //エネミー1
        world_.add_actor(new CarGhost{ &world_,GSvector3{70.0f,30.0f,0.0f} });
        //エネミー2
        world_.add_actor(new RushGhost{ &world_,GSvector3{100.0f,0.0f,0.0f} });
        //エネミー3
        world_.add_actor(new Poltergeist{ &world_,GSvector3{70.0f,-50.0f,0.0f} });
        //ボス
        world_.add_actor(new SurogSakones{ &world_,GSvector3{1.0f,1.0f,0.0f} });

        //シーン追加
        scene_.add("EnemyTestScene", new EnemyTestScene());
        scene_.add("BossTestScene", new BossTestScene());
        scene_.add("PlayerTestScene", new PlayerTestScene());
        scene_.add("TitleScene", new TitleScene());
        scene_.change("TitleScene");
        glError();
    }

    
    // 更新
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

   
    // 描画
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
    // 終了
    void end() override {
        world_.clear();
        // メッシュの削除
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

// メイン関数
int main() {
    return MyGame().run();
}