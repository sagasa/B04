#pragma once

// メッシュデータ
enum {
    Mesh_Player,                    // 自機
    Mesh_Enemy,                     // 敵
    Mesh_Asteroid01,                // 隕石１
    Mesh_Asteroid02,                // 隕石２
    Mesh_Asteroid03                 // 隕石３
};

// テクスチャデータ
enum {
    // 2Dスプライト
    Texture_BgTileNebulaGreen,      // 背景

    // 3Dスプライト
    Texture_EffectLazerCyan,        // レーザー（水色）
    Texture_EffectLazerOrange,      // レーザー（オレンジ色）
    Texture_EffectFlash             // 爆発フラッシュ
};

// ＢＧＭデータ
enum {
    Music_BackGround                // 爆発フラッシュ
};

// ＳＥデータ
enum {
    Se_ExplosionPlayer,             // 爆発音（自機）
    Se_ExplosionEnemy,              // 爆発音（敵）
    Se_ExplosionAsteroid,           // 爆発音（隕石）
    Se_WeaponPlayer,                // 発射音（自機）
    Se_WeaponEnemy,                 // 爆発音（敵）
};
