#include<gslib.h>
#include"BossTestScene.h"
#include"Assets.h"
#include"Field.h"
#include"Camera.h"
#include"Light.h"
#include"Player.h"
#include"SurogSakones.h"
#include "UI.h"
#include "AttackCollider.h"

void BossTestScene::start() {
    // プレーヤー弾画像を読み込み
    gsLoadTexture(Texture_EffectLazerOrange, "Assets/Effect/fx_lazer_orange_dff.png");
    // 敵弾画像を読み込み
    gsLoadTexture(Texture_EffectLazerCyan, "Assets/Effect/fx_lazer_cyan_dff.png");
    gsLoadTexture(Texture_Hp, "Assets/Image/outline_heart.png");
    gsLoadTexture(Texture_Shield, "Assets/Image/shield.png");
    // プレーヤメッシュの読み込み
    gsLoadMesh(Mesh_Player, "Assets/Model/Enemy/Ghost.msh");
    gsLoadMesh(Mesh_Poltergeist, "Assets/Model/Enemy/Ghost.msh");
    gsLoadSkeleton(Mesh_CarGhost, "Assets/Model/Enemy/Ghost.skl");
    gsLoadAnimation(Animation_CarGhost, "Assets/Model/Enemy/Ghost.anm");
    
    //SurogSakonesのメッシュの読み込み
    gsLoadMesh(Mesh_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.msh");
    //SurogSakonesのスケルトンの読み込み
    gsLoadSkeleton(Skeleton_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.skl");
    //SurogSakonesのアニメーションの読み込み
    gsLoadAnimation(Animation_SurogSakones, "Assets/Model/Enemy/Ghost_T-pose.anm");

    gsLoadMesh(Mesh_Paladin, "Assets/Model/Paladin/Paladin.msh");
    gsLoadSkeleton(Skeleton_Paladin, "Assets/Model/Paladin/Paladin.skl");
    gsLoadAnimation(Animation_Paladin, "Assets/Model/Paladin/Paladin.anm");

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
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
    //ボス
    world_.add_actor(new SurogSakones{ &world_,GSvector3{5.0f,0.0f,0.0f} });
    world_.add_actor(new UI{ &world_ });
}

//更新
void BossTestScene::update(float delta_time) {
	if(gsGetKeyTrigger(GKEY_RETURN))
	{
        BoundingSphere collider{ 0.5f,GSvector3{5.0f,-1.0f,0.0f} };
        world_.add_actor(new AttackCollider{ &world_,collider,"PlayerAttack","PlayerAttackName",60.0f,0.0f,10.0f });
	}
    world_.update(delta_time);
}
//描画
void BossTestScene::draw() const {
    world_.draw();
}
//終了しているか？
bool BossTestScene::is_end() const {
	return false;
}
//次のシーン名を返す
std::string BossTestScene::next() const {
	return "NullScene";
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