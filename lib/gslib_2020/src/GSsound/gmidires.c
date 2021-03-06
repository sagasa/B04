/******************************************************************************
* File Name : gmidires.c                           Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       ＭＩＤＩリソース管理．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"gmidires.h"
#include	"heap.h"

/****** データ型宣言 *********************************************************/

/*----- ＭＩＤＩリソース管理構造体 ------------------------------------------*/

typedef	struct
{
	GSOUND		gsnd;				/* サウンドハンドラ         */
	int			n;					/* ＭＩＤＩリソース数       */
	GMIDI**		midi;				/* ＭＩＤＩリソース数       */

} midires;

/*=============================================================================
*
* Purpose : ＭＩＤＩリソースの生成．
*
* Return  : ＭＩＤＩリソースハンドラを返す．
*
*============================================================================*/
GMIDIRESOURCE
GMidiResNew
(
	GSOUND			gsnd			/* サウンドハンドラ         */
)
{
	midires*		mres;

	mres = new_object( sizeof( midires ) );

	mres->gsnd = gsnd;
	mres->n    = 0;
	mres->midi = NULL;

	return	(GMIDIRESOURCE)mres;
}

/*=============================================================================
*
* Purpose : ＭＩＤＩリソースの消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GMidiResDelete
(
	GMIDIRESOURCE	hnd				/* ＭＩＤＩリソースハンドラ */
)
{
	midires*		mres = (midires*)hnd;

	/* すべてのＭＩＤＩリソースを解放 */
	GMidiResDelAllResource( hnd );

	del_object( mres );
}

/*=============================================================================
*
* Purpose : ＭＩＤＩリソースの初期化．
*
* Return  : なし．
*
*============================================================================*/
void
GMidiResInit
(
	GMIDIRESOURCE	hnd,			/* ＭＩＤＩリソースハンドラ */
	int				n				/* ＭＩＤＩリソース数       */
)
{
	midires*		mres = (midires*)hnd;
	int				i;

	/* すべてのＭＩＤＩリソースを解放 */
	GMidiResDelAllResource( hnd );

	mres->n    = n;
	mres->midi = NULL;

	if ( mres->n != 0 )
	{
		mres->midi = new_object( sizeof( GMIDI* ) * mres->n );
	}
	for ( i = 0; i < mres->n; i++ )
	{
		mres->midi[i] = NULL;
	}
}

/*=============================================================================
*
* Purpose : ＭＩＤＩリソースをすべて解放．
*
* Return  : なし．
*
*============================================================================*/
void
GMidiResDelAllResource
(
	GMIDIRESOURCE	hnd				/* ＭＩＤＩリソースハンドラ */
)
{
	midires*		mres = (midires*)hnd;
	int				i;

	if ( mres->midi == NULL )
	{
		return;
	}

	for ( i = 0; i < mres->n; i++ )
	{
		if ( mres->midi[i] != NULL )
		{
			GSoundDeleteMidi( mres->gsnd, mres->midi[i] );
		
			mres->midi[i] = NULL;
		}
	}
	del_object( mres->midi );

	mres->n    = 0;
	mres->midi = NULL;
}

/*=============================================================================
*
* Purpose : ＭＩＤＩリソースの読み込み．
*
* Return  : 読み込みに成功したら真，失敗したら偽を返す．
*
*============================================================================*/
BOOL
GMidiResLoadResource
(
	GMIDIRESOURCE	hnd,			/* ＭＩＤＩリソースハンドラ */
	int				id,				/* ＭＩＤＩリソースＩＤ     */
	const char*		midi_fname,		/* ＭＩＤＩファイル名       */
	BOOL			flag			/* ループ指定フラグ         */
)
{
	midires*		mres = (midires*)hnd;
	GMIDI			midi;

	if ( id >= mres->n )
	{
		return	FALSE;
	}

	if ( mres->midi[ id ] != NULL )
	{
		return	FALSE;
	}

	/* ＭＩＤＩファイルの読み込み */
	if ( ( midi = GSoundLoadMidi( mres->gsnd, midi_fname, flag ) ) == NULL )
	{
		return	FALSE;
	}
	mres->midi[ id ]= midi;

	return	TRUE;
}

/*=============================================================================
*
* Purpose : ＭＩＤＩリソースの取得．
*
* Return  : 読み込みに成功したら真，失敗したら偽を返す．
*
*============================================================================*/
GMIDI
GMidiResGetResource
(
	GMIDIRESOURCE	hnd,			/* ＭＩＤＩリソースハンドラ */
	int				id				/* ＭＩＤＩリソースＩＤ     */
)
{
	midires*		mres = (midires*)hnd;

	if ( id >= mres->n )
	{
		return	NULL;
	}

	return	mres->midi[ id ];
}

/*=============================================================================
*
* Purpose : ＭＩＤＩリソースの解放．
*
* Return  : なし．
*
*============================================================================*/
void
GMidiResDelResource
(
	GMIDIRESOURCE	hnd,			/* ＭＩＤＩリソースハンドラ */
	int				id				/* ＭＩＤＩリソースＩＤ     */
)
{
	midires*		mres = (midires*)hnd;

	if ( id >= mres->n )
	{
		return;
	}
	if ( mres->midi[ id ] == NULL )
	{
		return;
	}

	GSoundDeleteMidi( mres->gsnd, mres->midi[ id ] );

	mres->midi[ id ] = NULL;
}

/********** End of File ******************************************************/
