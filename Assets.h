#pragma once


// メッシュデータ
enum Mesh {
    Mesh_Player,                    // 自機
    Mesh_CarGhost,                  //CarGhost
    Mesh_RushGhost,                 //RushGhost
    Mesh_Poltergeist,               //Poltergeist
    Mesh_SurogSakones,               //SurogSakones
    Mesh_Skybox,
    Mesh_Paladin,
    Mesh_Book
};

//スケルトンデータ
enum Skeleton {
    Skeleton_Player,       //プレイヤー
    Skeleton_CarGhost,     //CarGhost
    Skeleton_RushGhost,    //RushGhost
    Skeleton_Poltergeist,  //Poltergeist
    Skeleton_SurogSakones, //SurogSakones
    Skeleton_Paladin
};

//アニメーションデータ
enum  Animation {
    Animation_Player,      //プレイヤー
    Animation_CarGhost,    //CarGhost
    Animation_RushGhost,   //RushGhost
    Animation_Poltergeist, //Poltergeist
    Animation_SurogSakones, //SurogSakones
    Animation_Paladin
};

// テクスチャデータ
enum Texture {
    //UI
    Texture_Hp,                     //体力(ハート)
    Texture_Shield,                 //体力(盾)
    Texture_TitleLogo,              //タイトルロゴ
    Texture_push_button,            //ボタン,
    Texture_Fade,                   //黒いテクスチャ
    // 3Dスプライト
    Texture_EffectLazerCyan,        // レーザー（水色）
    Texture_EffectLazerOrange,      // レーザー（オレンジ色）
    Texture_EffectFlash             // 爆発フラッシュ
};

// ＢＧＭデータ
enum Music {
    Music_BackGround                // BGM
};

// ＳＥデータ
enum Se {
    Se_ExplosionPlayer,             // 爆発音（自機）
    Se_ExplosionEnemy,              // 爆発音（敵）
    Se_ExplosionAsteroid,           // 爆発音（隕石）
    Se_WeaponPlayer,                // 発射音（自機）
    Se_WeaponEnemy,                 // 爆発音（敵）
};

enum Octree {
    Octree_Stage,
    Octree_Collider
};
