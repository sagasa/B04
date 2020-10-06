#include <GSgame.h> // gslib::Gameに必要
#include<iostream>

// ゲームクラス
class MyGame : public gslib::Game {
    // 開始
    void start() override {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
        using namespace std;
        cout << "Vendor :" << glGetString(GL_VENDOR) << '\n';
        cout << "GPU : " << glGetString(GL_RENDERER) << '\n';
        cout << "OpenGL ver. " << glGetString(GL_VERSION) << '\n';
       // cout << "【拡張機能一覧】" << endl;
       // cout << glGetString(GL_EXTENSIONS) << endl;

        // メッシュの読み込み
        gsLoadMesh(0, "Assets/Model/vehicle_playerShip.msh");
    }
    // 更新
    void update(float delta_time) override {
        ++i;
    }
    

    int i = 0;

    // 描画
    void draw() override {
        glPushMatrix();

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
        // メッシュの削除
        gsDeleteMesh(0);
    }
};

// メイン関数
int main() {
    return MyGame().run();
}