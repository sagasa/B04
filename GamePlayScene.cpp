#include"GamePlayScene.h"
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
#include"MapGenerator.h"
#include "player_ghost.h"


//開始
void GamePlayScene::start() {
    
    //生成
    MapGenerator generator{ &world_,"Assets/Map/Stage1.csv"};

    is_end_ = false;
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

    //Player
    gsLoadMesh(Mesh_Player, "Assets/Model/Enemy/Ghost.msh");
    gsLoadSkeleton(Skeleton_Player, "Assets/Model/Enemy/Ghost.skl");
    gsLoadAnimation(Animation_Player, "Assets/Model/Enemy/Ghost.anm");

    gsLoadMesh(Mesh_Paladin, "Assets/Model/Paladin/Paladin.msh");
    gsLoadSkeleton(Skeleton_Paladin, "Assets/Model/Paladin/Paladin.skl");
    gsLoadAnimation(Animation_Paladin, "Assets/Model/Paladin/Paladin.anm");
    //バレット(本)のメッシュの追加
    gsLoadMesh(Mesh_Book, "Assets/Model/Bullet/books.msh");
	
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
    world_.add_actor(new player_ghost{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
}

//更新
void GamePlayScene::update(float delta_time) {
	world_.update(delta_time);
}

//描画
void GamePlayScene::draw() const {
	world_.draw();
}

//終了しているか？
bool GamePlayScene::is_end() const {
	return world_.is_game_over()||world_.is_game_clear();
}

//次のシーンを返す
std::string GamePlayScene::next() const {
	return "TitleScene";
}
//終了
void GamePlayScene::end() {
	//ワールドを消去
	world_.clear();
    //メッシュの削除
    gsDeleteMesh(Mesh_Player);
    gsDeleteMesh(Mesh_Paladin);
    gsDeleteMesh(Mesh_CarGhost);
    gsDeleteMesh(Mesh_Poltergeist);
    gsDeleteMesh(Mesh_RushGhost);
    gsDeleteMesh(Mesh_Book);
    gsDeleteMesh(Mesh_Skybox);
    gsDeleteMesh(Mesh_SurogSakones);
    //スケルトンの削除
    gsDeleteSkeleton(Skeleton_CarGhost);
    gsDeleteSkeleton(Skeleton_Paladin);
    gsDeleteSkeleton(Skeleton_Poltergeist);
    gsDeleteSkeleton(Skeleton_RushGhost);
    gsDeleteSkeleton(Skeleton_SurogSakones);
    //アニメーションの削除
    gsDeleteAnimation(Animation_CarGhost);
    gsDeleteAnimation(Animation_Paladin);
    gsDeleteAnimation(Animation_Poltergeist);
    gsDeleteAnimation(Animation_RushGhost);
    gsDeleteAnimation(Animation_SurogSakones);
    //オクツリーの削除
    gsDeleteOctree(Octree_Collider);
    gsDeleteOctree(Octree_Stage);
	
	
}