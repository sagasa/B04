#ifndef _GS_MUSIC_H_
#define _GS_MUSIC_H_

#include <gstype.h>

#ifdef __cplusplus
extern "C" {
#endif

// 音楽の初期化
void gsInitMusic(void);

// ウィンドがアクティブの場合の処理
void gsActivateMusic(GSboolean active);

// 音楽の終了処理
void gsFinishMusic(void);

// クリーンアップ
void gsCleanupMusic(void);

// 読み込み
GSboolean gsLoadMusic(GSuint musicID, const char* fileName, GSboolean loop);

// 音楽のバインド
void gsBindMusic(GSuint musicID);

// 削除
void gsDeleteMusic(GSuint musicID);

// 再生
void gsPlayMusic(void);

// 一時停止
void gsPauseMusic(void);

// リスタート
void gsRestartMusic(void);

// 停止
void gsStopMusic(void);

// 再生中か？
GSboolean gsIsPlayMusic(void);

// ポーズ中か？
GSboolean gsIsPauseMusic(void);

// ループ再生しているか
GSboolean gsIsLoopMusic(void);

// 再生終了しているか？
GSboolean gsIsEndMusic(void);

// ボリュームの設定
void gsSetMusicVolume(GSfloat volume);

// バランスの設定
void gsSetMusicBalance(GSfloat balance);

// 再生タイマの設定
void gsSetMusicTime(float time);

// 現在の再生時間の取得
GSfloat gsGetMusicTime(void);

// 終了時間の取得
GSfloat gsGetMusicEndTime(void);

// ボリュームの取得
GSfloat gsGetMusicVolume(void);

// バランスの取得
GSfloat gsGetMusicBalance(void);


#ifdef __cplusplus
}
#endif

#ifdef	NDEBUG
# ifdef _MT
#  ifdef _DLL
#   pragma comment( lib, "gslibEXT_md.lib" )
#  else
#   pragma comment( lib, "gslibEXT_mt.lib" )
#  endif
# endif
#else
# ifdef _MT
#  ifdef _DLL
#   pragma comment( lib, "gslibEXT_mdd.lib" )
#  else
#   pragma comment( lib, "gslibEXT_mtd.lib" )
#  endif
# endif
#endif

#endif
