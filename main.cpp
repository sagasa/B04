#include <GSgame.h>
#include "Assets.h"
#include "ActorManager.h"
#include "Player.h"
#include "Light.h"
#include "Camera.h"
#include "Field.h"
#include "World.h"
#include"SurogSakones.h"
#include <iostream>

// ゲームクラス
class MyGame : public gslib::Game {
    // ワールドクラス
    World world_;
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
        // 敵メッシュの読み込み
        gsLoadMesh(Mesh_Enemy, "Assets/Model/vehicle_enemyShip.msh");
        // 隕石メッシュの読み込み
        gsLoadMesh(Mesh_Asteroid01, "Assets/Model/prop_asteroid_01.msh");
        gsLoadMesh(12, "Assets/Model/Enemy/Ghost_T-Pose.msh");
        gsLoadSkeleton(12, "Assets/Model/Enemy/Ghost_T-Pose.skl");
        gsLoadAnimation(12, "Assets/Model/Enemy/Ghost_T-Pose.anm");


        // フィールドの追加
        world_.add_field(new Field{ Texture_BgTileNebulaGreen });
        // カメラの追加
        world_.add_camera(new Camera{ &world_ });
        // ライトの追加
        world_.add_light(new Light{ &world_ });
        // プレーヤの追加
        world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
        world_.add_actor(new SurogSakones{ &world_,GSvector3{0.0f,0.0f,0.0f} });
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
        world_.draw();
        //
        // 自機がｙ軸プラス方向を向くように回転させる
        glRotatef(i % 360, 0.0f, 1.0f, 0.0f);   // y軸まわりに180°回転
        glRotatef(90, 1.0f, 0.0f, 0.0f);   // x軸まわりに-90°回転
        // メッシュ描画
        gsDrawMesh(0);
        glPopMatrix();
        
    }
    // 終了
    void end() override {
        world_.clear();
        // メッシュの削除
        gsDeleteMesh(0);
    }
};

// メイン関数
int main() {
    return MyGame().run();
}