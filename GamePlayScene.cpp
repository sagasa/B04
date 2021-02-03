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
#include"player_paladin.h"
#include<string>
#include<sstream>



//開始
void GamePlayScene::start(int number) {
    fade_.start(true,1);
    stage_number_ = number;

    gsLoadShader(0, "Paladin.vert", "Paladin.frag");
    // ぱっちぃメッシュの読み込み
    //gsLoadMeshEx(0, "Assets/Model/patti.msh");
    //終了フラグ初期化
    is_end_ = false;
    //表示タイマーの初期化
    draw_timer_ = 0.0f;
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

    gsLoadMesh(Mesh_Paladin, "Assets/Model/Paladin_j_nordstrom/paladin_j_nordstrom.msh");
    gsLoadSkeleton(Skeleton_Paladin, "Assets/Model/Paladin_j_nordstrom/paladin_j_nordstrom.skl");
    gsLoadAnimation(Animation_Paladin, "Assets/Model/Paladin_j_nordstrom/paladin_j_nordstrom.anm");
    //バレット(本)のメッシュの追加
    gsLoadMesh(Mesh_Book, "Assets/Model/Bullet/books.msh");
	
    //スカイボックスの読み込み
    gsLoadMesh(Mesh_Skybox, "Assets/Skybox/DarkStorm4K.msh");
    //int型のステージ番号をstringstream型に変換
    std::stringstream ss;
    ss << stage_number_;
    //ステージ番号によってロードするCSVファイルを変更する
    std::string csv_pass;
    csv_pass = "Assets/Map/Stage" + ss.str() + ".csv";
    //生成
    MapGenerator generator{ &world_,csv_pass};
    //MapGenerator generator{ &world_,"Assets/Map/Stage3.csv" };
    //番号によってロードするマップを変える
    std::string octree_pass;
    octree_pass = "Assets/Octree/stage"+ ss.str() +"/stage"+ ss.str()+".oct";
    std::string collide_pass = "Assets/Octree/stage" + ss.str() + "/stage" + ss.str() + "_collide.oct";
    //描画用オクツリーの読み込み
    gsLoadOctree(Octree_Stage, octree_pass.c_str());
    //衝突判定用オクツリーの読み込み
    gsLoadOctree(Octree_Collider, collide_pass.c_str());
    //描画用オクツリーの読み込み
    /*gsLoadOctree(Octree_Stage, "Assets/Octree/stage3/stage3.oct");
    //衝突判定用オクツリーの読み込み
    gsLoadOctree(Octree_Collider, "Assets/Octree/stage3/stage3.oct");*/

	//パーティクル用のテクスチャ
    gsLoadTexture(Texture_Smoke, "Assets/Effect/particle_smoke.png");
    gsLoadTexture(Texture_Hit, "Assets/Effect/particle_glitter_3.png");
    gsLoadTexture(Texture_Circle, "Assets/Effect/particle_dot_1.png");
    gsLoadTexture(Texture_BossHP, "Assets/Image/BossHP.png");
    gsLoadTexture(Texture_BossHPFrame, "Assets/Image/BossHPFrame.png");
    gsLoadTexture(Texture_BossHPFrame_Lid, "Assets/Image/BossHPFrame_Lid.png");

	//SE
    gsLoadSE(SE_Attack, "Assets/SE/punch.wav", 1, false);
    gsLoadSE(SE_Jump, "Assets/SE/Jump.wav",1,false);
    gsLoadSE(SE_GhostDamage, "Assets/SE/ghost_damage.wav", 1, false);
    gsLoadSE(SE_ParadinDamage, "Assets/SE/paradin_damage.wav", 1, false);
    //gsLoadSE(SE_GhostDamage, "Assets/SE/punch.wav", 1, false);
    //gsLoadSE(SE_ParadinDamage, "Assets/SE/Jump.wav", 1, false);
    gsLoadSE(SE_P_Ghost_Attack, "Assets/SE/p_ghost_Attack.wav", 1, false);

    gsLoadTexture(Texture_Hp, "Assets/Image/heart.dds");
    gsLoadTexture(Texture_Shield, "Assets/Image/shield.dds");
    
    gsLoadSE(SE_GhostAttack1, "Assets/SE/Ghost Attack_01.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_GhostAttack2, "Assets/SE/Ghost Attack_02.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_BossGhostDamage, "Assets/SE/Ghost Damaged_01.wav", 1, GWAVE_DEFAULT);
    gsLoadSE(SE_GhostDeath, "Assets/SE/Ghost Death_02.wav", 1, GWAVE_DEFAULT);

    gsLoadSE(SE_Slash, "Assets/SE/sword-drawn1.wav", 1, GWAVE_DEFAULT);

    gsLoadSE(SE_Shoot, "Assets/SE/shoot.wav", 1, GWAVE_DEFAULT);
	
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
    //音量調整
    gsSetMusicVolume(0.8f);
    //ワールドの更新
	world_.update(delta_time);
    //フェードクラスの更新
    fade_.update(delta_time);

    if (fade_.is_end()) {
        //表示タイマーの更新
        draw_timer_ += delta_time;
    }

    if (!is_end_ && world_.is_game_clear() && fade_.is_end()) {//ボスが死んだか？
        is_end_ = true;
        next_scene_ = "ResultScene";
        fade_.change_fade_flg();
        next_scene_ = (stage_number_ == 3) ? "ResultScene" : "StageClearScene";
    }
    else if (!is_end_ && world_.is_game_over() && fade_.is_end()) {//プレイヤーが死んだか？
        is_end_ = true;
        next_scene_ = "GameOverScene";
        fade_.change_fade_flg();
    }
    
}

//描画
void GamePlayScene::draw() const {
    //ワールドの描画
	world_.draw();
    //ステージ名を表示

    //フェードクラスの描画
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

//現在のステージ番号を返す
int GamePlayScene::stage_number() const {
    return stage_number_;
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

    gsDeleteSE(SE_Attack);
    gsDeleteSE(SE_Jump);
    gsDeleteSE(SE_ParadinDamage);
    gsDeleteSE(SE_GhostDeath);
    gsDeleteSE(SE_GhostDamage);
    gsDeleteSE(SE_GhostAttack1);	
    gsDeleteSE(SE_GhostAttack2);	
    gsDeleteSE(SE_BossGhostDamage);
    gsDeleteSE(SE_Shoot);
    gsDeleteSE(SE_Slash);
    gsDeleteSE(SE_P_Ghost_Attack);
    gsDeleteTexture(Texture_BossHPFrame);
    gsDeleteTexture(Texture_BossHP);
    gsDeleteTexture(Texture_BossHPFrame_Lid);
    gsDeleteTexture(Texture_Circle);
    gsDeleteTexture(Texture_Smoke);
    gsDeleteTexture(Texture_Hit);
}