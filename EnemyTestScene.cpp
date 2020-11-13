#include"EnemyTestScene.h"
#include"Player.h"
#include"CarGhost.h"
#include"RushGhost.h"
#include"Poltergeist.h"
#include"Field.h"
#include"Camera.h"
#include"Light.h"
#include"Assets.h"


// ワールドクラス
World world_;

//開始
void EnemyTestScene::start() {
    // フィールドの追加
    world_.add_field(new Field{ Texture_BgTileNebulaGreen });
    // カメラの追加
    world_.add_camera(new Camera{ &world_ });
    // ライトの追加
    world_.add_light(new Light{ &world_ });
    // プレーヤの追加
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
    //エネミー1
    world_.add_actor(new CarGhost{ &world_,GSvector3{70.0f,30.0f,0.0f} });
    //エネミー2
    world_.add_actor(new RushGhost{ &world_,GSvector3{100.0f,0.0f,0.0f} });
    //エネミー3
    world_.add_actor(new Poltergeist{ &world_,GSvector3{70.0f,-50.0f,0.0f} });
}

//更新
void EnemyTestScene::update(float delta_time) {
    world_.update(delta_time);
}

//描画
void EnemyTestScene::draw() const {
    world_.draw();
}

//終了しているか
bool EnemyTestScene::is_end() const {
	return false;
}

//次のシーンを返す
/*std::string EnemyTestScene::next() const {

}*/

//終了
void EnemyTestScene::end() {
    world_.clear();
    // メッシュの削除
    gsDeleteMesh(Mesh_Player);
    gsDeleteMesh(Mesh_CarGhost);
    gsDeleteMesh(Mesh_RushGhost);
    gsDeleteMesh(Mesh_Poltergeist);
}