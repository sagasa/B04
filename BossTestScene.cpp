#include<gslib.h>
#include"BossTestScene.h"

#include <iostream>

#include"Assets.h"
#include"Field.h"
#include"Camera.h"
#include"Light.h"
#include"Player.h"
#include "player_ghost.h"
#include "player_paladin.h"
#include"SurogSakones.h"
#include "UI.h"
#include "AttackCollider.h"
#include "Actor.h"
#include "Particle.h"
#include "ParticleManager.h"

void BossTestScene::start() {
    // プレーヤー弾画像を読み込み
    gsLoadTexture(Texture_EffectLazerOrange, "Assets/Effect/fx_lazer_orange_dff.png");
    // 敵弾画像を読み込み
    gsLoadTexture(Texture_EffectLazerCyan, "Assets/Effect/fx_lazer_cyan_dff.png");
    gsLoadTexture(Texture_Hp, "Assets/Image/outline_heart.png");
    gsLoadTexture(Texture_Shield, "Assets/Image/shield.png");
    //gsLoadTexture(Texture_Distotion, "Assets/Effect/ShockWaveDistortion1.png");
    gsLoadTexture(Texture_Smoke, "Assets/Effect/particle_smoke.png");
    //Player
    gsLoadMesh(Mesh_Player, "Assets/Model/Enemy/Ghost.msh");
    gsLoadSkeleton(Skeleton_Player, "Assets/Model/Enemy/Ghost.skl");
    gsLoadAnimation(Animation_Player, "Assets/Model/Enemy/Ghost.anm");

    gsLoadMesh(Mesh_Paladin, "Assets/Model/Paladin/Paladin.msh");
    gsLoadSkeleton(Skeleton_Paladin, "Assets/Model/Paladin/Paladin.skl");
    gsLoadAnimation(Animation_Paladin, "Assets/Model/Paladin/Paladin.anm");
    
    //SurogSakonesのメッシュの読み込み
    gsLoadMesh(Mesh_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.msh");
    //SurogSakonesのスケルトンの読み込み
    gsLoadSkeleton(Skeleton_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.skl");
    //SurogSakonesのアニメーションの読み込み
    gsLoadAnimation(Animation_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.anm");

    gsLoadOctree(Octree_Stage, "Assets/Octree/stage1.oct");
    gsLoadOctree(Octree_Collider, "Assets/Octree/stage1_collider.oct");
    gsLoadMesh(Mesh_Skybox, "Assets/Skybox/skydome.msh");

    //バレット(本)のメッシュの追加
    gsLoadMesh(Mesh_Book, "Assets/Model/Bullet/books.msh");
	
    // フィールドの追加
    world_.add_field(new Field{ Octree_Stage,Octree_Collider,Mesh_Skybox });
    // カメラの追加
    world_.add_camera(new Camera{ &world_ });
    // ライトの追加
    world_.add_light(new Light{ &world_ });
    // プレーヤの追加
    world_.add_actor(new player_paladin{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
    //ボス
    //ボス
    // プレーヤの追加
    world_.add_actor(new player_ghost{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
    world_.add_particle_manager(new ParticleManager{ &world_ });
	
}

//更新
void BossTestScene::update(float delta_time) {
	if(gsGetKeyState(GKEY_1))
	{
        world_.particle_manager()->smoke(GSvector3::zero());
	}
	if(gsGetKeyTrigger(GKEY_2))
	{
        world_.particle_manager()->distotion(GSvector3::zero());
	}
	if(gsGetKeyTrigger(GKEY_RETURN))
	{
        world_.particle_manager()->spark(GSvector3::zero());
        /*Particle* p = new Particle{ &world_ };
        p->transform().position(GSvector3{ 0.0f,0.0f,0.0f });
        p->velocity(GSvector3{ 0.05f,0.05f,0.05f });
        p->lifespan_= 30.0f ;
        p->image_handle_ = Texture_EffectLazerCyan;
        p->start_scale_ = GSvector2::zero();
        p->end_scale_ = GSvector2::one();
        p->damp_ = 1.0f;
        p->OnUpdate = [] {std::cout<<"OnUpdate"<<std::endl; return; };
        world_.add_actor(p);*/
	}
    world_.update(delta_time);
}
//描画
void BossTestScene::draw() const {
    world_.draw();
}
//終了しているか？
bool BossTestScene::is_end() const {
	return world_.is_game_clear()||world_.is_game_over();
}
//次のシーン名を返す
std::string BossTestScene::next() const {
	return "TitleScene";
}
//終了
void BossTestScene::end() {
    world_.clear();
    gsDeleteMesh(Mesh_Player);
    gsDeleteMesh(Mesh_SurogSakones);
    gsDeleteSkeleton(Skeleton_SurogSakones);
    gsDeleteAnimation(Animation_SurogSakones);
    gsDeleteTexture(Texture_EffectFlash);
    gsDeleteTexture(Texture_EffectLazerCyan);
    gsDeleteTexture(Texture_EffectLazerOrange);
    gsDeleteMesh(Mesh_Skybox);
    gsDeleteOctree(Octree_Stage);
    gsDeleteOctree(Octree_Collider);
}