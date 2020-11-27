#include"EnemyTestScene.h"
#include"Player.h"
#include"CarGhost.h"
#include"RushGhost.h"
#include"Poltergeist.h"
#include"NormalGhost.h"
#include"Field.h"
#include"Camera.h"
#include"Light.h"
#include"Assets.h"
#include"SceneManager.h"
#include"SurogSakones.h"


//開始
void EnemyTestScene::start() {
    // CarGhostのメッシュの読み込み
    gsLoadMesh(Mesh_CarGhost, "Assets/Model/Enemy/Ghost.msh");
    gsLoadMesh(Mesh_RushGhost, "Assets/Model/Enemy/Ghost2.msh");
    gsLoadMesh(Mesh_Poltergeist, "Assets/Model/Enemy/Ghost3.msh");
    gsLoadSkeleton(Skeleton_CarGhost, "Assets/Model/Enemy/Ghost.skl");
    gsLoadSkeleton(Skeleton_RushGhost, "Assets/Model/Enemy/Ghost.skl");
    gsLoadSkeleton(Skeleton_Poltergeist, "Assets/Model/Enemy/Ghost.skl");
    gsLoadAnimation(Animation_CarGhost, "Assets/Model/Enemy/Ghost.anm");
    gsLoadAnimation(Animation_RushGhost, "Assets/Model/Enemy/Ghost.anm");
    gsLoadAnimation(Animation_Poltergeist, "Assets/Model/Enemy/Ghost.anm");

    //SurogSakonesのメッシュの読み込み
    gsLoadMesh(Mesh_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.msh");
    //SurogSakonesのスケルトンの読み込み
    gsLoadSkeleton(Skeleton_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.skl");
    //SurogSakonesのアニメーションの読み込み
    gsLoadAnimation(Animation_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.anm");

    //スカイボックスの読み込み
    gsLoadMesh(Mesh_Skybox, "Assets/Skybox/skydome.msh");
    //描画用オクツリーの読み込み
    gsLoadOctree(Octree_Stage, "Assets/Octree/stage1.oct");
    //衝突判定用オクツリーの読み込み
    gsLoadOctree(Octree_Collider, "Assets/Octree/stage1_collider.oct");

    // フィールドの追加
    world_.add_field(new Field{ Octree_Stage,Octree_Collider,Mesh_Skybox });
    // カメラの追加
    world_.add_camera(new Camera{ &world_ });
    // ライトの追加
    world_.add_light(new Light{ &world_ });
    // プレーヤの追加
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
    //エネミー1
    world_.add_actor(new CarGhost{ &world_,GSvector3{10.0f,0.0f,0.0f} });
    //エネミー2
    world_.add_actor(new RushGhost{ &world_,GSvector3{-10.0f,0.0f,0.0f} });
    //エネミー3
    world_.add_actor(new Poltergeist{ &world_,GSvector3{5.0f,0.0f,0.0f} });
    //エネミー4
    world_.add_actor(new NormalGhost{ &world_,GSvector3{3.0f,5.0f,0.0f} });
    world_.add_actor(new SurogSakones{ &world_,GSvector3{15.0f,0.0f,0.0f} });
}

//更新
void EnemyTestScene::update(float delta_time) {
    world_.update(delta_time);
    if (gsGetKeyTrigger(GKEY_RETURN)) {
    }
}

//描画
void EnemyTestScene::draw() const {
    world_.draw();
}

//終了しているか
bool EnemyTestScene::is_end() const {
	return is_end_;
}

//次のシーンを返す
std::string EnemyTestScene::next() const {
    return "scene_null";

}

//終了
void EnemyTestScene::end() {
    world_.clear();
}