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
#include"ParticleManager.h"



//開始
void GamePlayScene::start() {
    fade_.start(true,1);

    gsLoadShader(0, "Paladin.vert", "Paladin.frag");
    // ぱっちぃメッシュの読み込み
    gsLoadMeshEx(0, "Assets/Model/patti.msh");
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
    gsLoadMesh(Mesh_Skybox, "Assets/Skybox/DarkStorm4K.msh");
    //描画用オクツリーの読み込み
    gsLoadOctree(Octree_Stage, "Assets/Octree/stage1.oct");
    //衝突判定用オクツリーの読み込み
    gsLoadOctree(Octree_Collider, "Assets/Octree/stage1_collider.oct");

    gsLoadSE(SE_Attack, "Assets/SE/punch.wav", 1, false);
    gsLoadSE(SE_Jump, "Assets/SE/Jump.wav",1,false);
    gsLoadSE(SE_GhostDamage, "Assets/SE/ghost_damage.wav", 1, false);
    gsLoadSE(SE_ParadinDamage, "Assets/SE/paradin_damage.wav", 1, false);
    //gsLoadSE(SE_GhostDamage, "Assets/SE/punch.wav", 1, false);
    //gsLoadSE(SE_ParadinDamage, "Assets/SE/Jump.wav", 1, false);
	
    gsLoadMusic(Music_GamePlay, "Assets/BGM/gameplay.wav", GS_TRUE);
    gsBindMusic(Music_GamePlay);
    gsPlayMusic();

    // フィールドの追加
    world_.add_field(new Field{ Octree_Stage,Octree_Collider,Mesh_Skybox });
    // カメラの追加
    world_.add_camera(new Camera{ &world_ });
    // ライトの追加
    world_.add_light(new Light{ &world_ });
    // プレーヤの追加
    world_.add_actor(new player_ghost{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
    //追加
    world_.add_particle_manager(new ParticleManager{ &world_ });
}

//更新
void GamePlayScene::update(float delta_time) {
    gsSetMusicVolume(0.8f);
	world_.update(delta_time);

    if (world_.is_game_clear() && fade_.is_end()) {//ボスが死んだか？
        is_end_ = true;
        next_scene_ = "ResultScene";
        fade_.change_fade_flg();
    }
    else if (world_.is_game_over() && fade_.is_end()) {//プレイヤーが死んだか？
        is_end_ = true;
        next_scene_ = "GameOverScene";
        fade_.change_fade_flg();
    }
    fade_.update(delta_time);
}

//描画
void GamePlayScene::draw() const {
	world_.draw();
    fade_.draw();
}

//終了しているか？
bool GamePlayScene::is_end() const {
    if (fade_.is_end()) {
        return is_end_;
    }
    else return false;//終了フラグを返す
}

//次のシーンを返す
std::string GamePlayScene::next() const {
	return next_scene_;
}
//終了
void GamePlayScene::end() {
    gsStopMusic();
    gsDeleteMusic(Music_Title);
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