#include<gslib.h>
#include"PlayerTestScene.h"
#include"Assets.h"
#include"Field.h"
#include"Camera.h"
#include"Light.h"
#include"Player.h"
#include "resource_loader.h"
#include"SurogSakones.h"

void PlayerTestScene::load(resource_loader& loader)
{
    // プレーヤメッシュの読み込み
    //loader.load_mesh(Mesh_Player, "Assets/Model/Enemy/Ghost.msh");
    //loader.load_skeleton(Skeleton_Player, "Assets/Model/Enemy/Ghost.skl");
    //loader.load_animation(Animation_Player, "Assets/Model/Enemy/Ghost.anm");

    //loader.load_mesh(Mesh_Paladin, "Assets/Model/Paladin/Paladin.msh");
    //loader.load_skeleton(Skeleton_Paladin, "Assets/Model/Paladin/Paladin.skl");
    //loader.load_animation(Animation_Paladin, "Assets/Model/Paladin/Paladin.anm");

 	
    //loader.load_octree(Octree_Stage, "Assets/Octree/stage1.oct");
    //loader.load_octree(Octree_Collider, "Assets/Octree/stage1_collider.oct"); //なぜかこっちで読んでいるのに消える
    //loader.load_mesh(Mesh_Skybox, "Assets/Skybox/skydome.msh");
}

void PlayerTestScene::start() {
    is_end_ = false;

    gsLoadMesh(Mesh_Player, "Assets/Model/Enemy/Ghost.msh");
    gsLoadSkeleton(Skeleton_Player, "Assets/Model/Enemy/Ghost.skl");
    gsLoadAnimation(Animation_Player, "Assets/Model/Enemy/Ghost.anm");

    gsLoadMesh(Mesh_Paladin, "Assets/Model/Paladin/Paladin.msh");
    gsLoadSkeleton(Skeleton_Paladin, "Assets/Model/Paladin/Paladin.skl");
    gsLoadAnimation(Animation_Paladin, "Assets/Model/Paladin/Paladin.anm");
	
    gsLoadOctree(Octree_Stage, "Assets/Octree/stage1.oct");
    gsLoadOctree(Octree_Collider, "Assets/Octree/stage1_collider.oct"); 
    gsLoadMesh(Mesh_Skybox, "Assets/Skybox/skydome.msh");

	// フィールドの追加
    world_.add_field(new Field{ Octree_Stage,Octree_Collider,Mesh_Skybox });
    // カメラの追加
    world_.add_camera(new Camera{ &world_ });
    // ライトの追加
    world_.add_light(new Light{ &world_ });
    // プレーヤの追加
    world_.add_actor(new Player{ &world_, GSvector3{ 0.0f, 0.0f, 0.0f } });
}

//更新
void PlayerTestScene::update(float delta_time) {
    world_.update(delta_time);
}
//描画
void PlayerTestScene::draw() const {
    world_.draw();
}
//終了しているか？
bool PlayerTestScene::is_end() const {
    return is_end_;
}
//次のシーン名を返す
std::string PlayerTestScene::next() const {
    return "NullScene";
}
//終了
void PlayerTestScene::end() {
    world_.clear();
    gsDeleteMesh(Mesh_Player);
    gsDeleteMesh(Mesh_SurogSakones);
    gsDeleteSkeleton(Skeleton_SurogSakones);
    gsDeleteAnimation(Animation_SurogSakones);
	
    gsDeleteTexture(Texture_BgTileNebulaGreen);
    gsDeleteTexture(Texture_EffectFlash);
    gsDeleteTexture(Texture_EffectLazerCyan);
    gsDeleteTexture(Texture_EffectLazerOrange);
    gsDeleteMesh(Mesh_Skybox);
    gsDeleteOctree(Octree_Stage);
    gsDeleteOctree(Octree_Collider);
}