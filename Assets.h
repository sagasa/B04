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
    Mesh_Book,
    Mesh_Ghost
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

//シェーダーデータ
enum Shader {
    Shader_Ghost,
    Shader_Paladin,
    Shader_Map
};

// テクスチャデータ
enum Texture {
    //UI
    Texture_Hp,                     //体力(ハート)
    Texture_Shield,                 //体力(盾)
    //文字
    Texture_TitleLogo,              //タイトルロゴ
    Texture_PushButton,            //ボタン
    Texture_GameClear,                  //ゲームクリア
    Texture_GameOver,              //ゲームオーバー
    Texture_TitleButton,        //タイトルへ戻る
    Texture_RestartButton,               //もう一度
    Texture_NextStage,             //次のステージへ
    //フェード
    Texture_Fade,                   //黒いテクスチャ
    // 3Dスプライト
    Texture_EffectLazerCyan,        // レーザー（水色）
    Texture_EffectLazerOrange,      // レーザー（オレンジ色）
    Texture_EffectFlash,             // 爆発フラッシュ
    Texture_Smoke,
	Texture_Distotion,
	Texture_Hit,
	Texture_Circle,
	Texture_BossHP
};

// ＢＧＭデータ
enum Music {
    Music_Title,                    //タイトルBGM
    Music_GamePlay,                 //ゲームプレイBGM
    Music_GameOver,                 //ゲームオーバーBGM
    Music_GameClear                 //ゲームクリアBGM
};

// ＳＥデータ
enum Se {
    SE_GhostDamage,                 //幽霊のダメージSE
    SE_GhostMove,                   //幽霊の移動SE
    SE_ParadinMove,                 //憑依の移動SE
    SE_ParadinDamage,               //憑依のダメージSE
    SE_Attack,                      //攻撃SE
    SE_Jump,                        //ジャンプSE
    SE_Transfer,                    //憑依SE
    SE_Slash,                       //斬撃SE
    SE_Shoot,                       //発射SE
    SE_Select,                      //選択移動SE
    SE_Push                         //決定SE

};

enum Octree {
    Octree_Stage,
    Octree_Collider
};
