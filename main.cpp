#include <condition_variable>
#include <GSgame.h>
#include "Assets.h"
#include "World.h"
#include <iostream>
#include <queue>

#include "ByteBuf.h"
#include"SceneManager.h"
#include"EnemyTestScene.h"
#include"BossTestScene.h"
#include"GamePlayScene.h"
#include "CarGhost.h"
#include "Player.h"
#include"TitleScene.h"
#include"PlayerTestScene.h"
#include "resource_loader.h"
#include "thread_pool.h"
#include"ActorProp.h"


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

        //シーン追加
        scene_.add("EnemyTestScene", new EnemyTestScene());
        scene_.add("BossTestScene", new BossTestScene());
        scene_.add("PlayerTestScene", new PlayerTestScene());
        scene_.add("TitleScene", new TitleScene());
        scene_.add("GamePlayScene", new GamePlayScene());
        scene_.change("TitleScene");
        glError();        
    }

	
    // 更新
    void update(float delta_time) override {
        //world_.update(delta_time);

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
        //gsDrawMesh(Mesh_Player);
        glPopMatrix();
    	
        glPushMatrix();
        using namespace std;

        scene_.draw();


        glPopMatrix();
        glError();
    }
    // 終了
    void end() override {
        world_.clear();
    }
public:
    MyGame() : Game(1920, 1080, false, 60) {

    }

};

// メイン関数
int main() {
	return MyGame().run();
}