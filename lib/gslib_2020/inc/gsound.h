/******************************************************************************
* File Name : gsound.h                             Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       ゲームシステム用 サウンドデータ管理 ヘッダファイル．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSOUND_H_
#define		_GSOUND_H_

#include	"gwave.h"
#include	"gmidi.h"

/****** データ型宣言 *********************************************************/

typedef	void*		GSOUND;			/* サウンドハンドラ             */

/****** 関数プロトタイプ宣言 *************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*=============================================================================
*
* Purpose : サウンドハンドラの生成．
*
* Return  : サウンドハンドラを返す．
*
*============================================================================*/
GSOUND
GSoundNew
(
	HWND			hwnd			/* ウィンドウハンドラ           */
);

/*=============================================================================
*
* Purpose : サウンドハンドラの消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundDelete
(
	GSOUND			hnd				/* サウンドハンドラ             */
);

/*=============================================================================
*
* Purpose : 全てのサウンドの停止．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundStop
(
	GSOUND			hnd				/* サウンドハンドラ             */
);

/*=============================================================================
*
* Purpose : 全てのＷＡＶＥサウンドの停止．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundStopWave
(
	GSOUND			hnd				/* サウンドハンドラ             */
);

/*=============================================================================
*
* Purpose : 全てのＭＩＤＩサウンドの停止．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundStopMidi
(
	GSOUND			hnd				/* サウンドハンドラ             */
);

/*=============================================================================
*
* Purpose : 全てのサウンドデータの消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundCleanUp
(
	GSOUND			hnd				/* サウンドハンドラ             */
);

/*=============================================================================
*
* Purpose : 全てのウェーブデータを消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundCleanUpWave
(
	GSOUND			hnd				/* サウンドハンドラ             */
);

/*=============================================================================
*
* Purpose : 全てのＭＩＤＩデータを消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundCleanUpMidi
(
	GSOUND			hnd				/* サウンドハンドラ             */
);

/*=============================================================================
*
* Purpose : ＷＡＶＥファイルの読み込み．
*
* Return  : ＷＡＶＥデータハンドラを返す．
*
*============================================================================*/
GWAVE
GSoundLoadWave
(
	GSOUND			hnd,			/* サウンドハンドラ             */
	const char*		fname,			/* ＷＡＶＥファイル名           */
	int				cnt,			/* 同時再生数                   */
	GWAVEFLAG		flag			/* 再生タイプフラグ             */
);

/*=============================================================================
*
* Purpose : ＷＡＶＥデータの消滅．
*
* Return  : 消滅できれば真，できなければ偽を返す．
*
*============================================================================*/
BOOL
GSoundDeleteWave
(
	GSOUND			hnd,			/* サウンドハンドラ             */
	GWAVE			wave			/* ＷＡＶＥデータハンドラ       */
);

/*=============================================================================
*
* Purpose : ＭＩＤＩファイルの読み込み．
*
* Return  : ＭＩＤＩデータハンドラを返す．
*
*============================================================================*/
GMIDI
GSoundLoadMidi
(
	GSOUND			hnd,			/* サウンドハンドラ             */
	const char*		fname,			/* ＭＤＳファイル名             */
	GMIDIFLAG		flag			/* 再生タイプフラグ             */
);

/*=============================================================================
*
* Purpose : ＭＩＤＩデータの消滅．
*
* Return  : 消滅できれば真，できなければ偽を返す．
*
*============================================================================*/
BOOL
GSoundDeleteMidi
(
	GSOUND			hnd,			/* サウンドハンドラ             */
	GMIDI			midi			/* ＭＩＤＩデータハンドラ       */
);

/*=============================================================================
*
* Purpose : サウンドがアクティブになった時の処理．
*
* Return  : なし．
*
*============================================================================*/
void
GSoundActivateApp
(
	GSOUND			hnd,			/* サウンドハンドラ             */
	BOOL			active			/* アクティブフラグ             */
);

#ifdef __cplusplus
}
#endif

#endif

/********** End of File ******************************************************/
