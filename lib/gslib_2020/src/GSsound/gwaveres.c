/******************************************************************************
* File Name : gwaveres.c                           Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       ＷＡＶＥリソース管理．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"gwaveres.h"
#include	"heap.h"

/****** データ型宣言 *********************************************************/

/*----- ＷＡＶＥリソース管理構造体 ------------------------------------------*/

typedef	struct
{
	GSOUND		gsnd;				/* サウンドハンドラ         */
	int			n;					/* ＷＡＶＥリソース数       */
	GWAVE**		wave;				/* ＷＡＶＥリソース         */

} waveres;

/*=============================================================================
*
* Purpose : ＷＡＶＥリソースの生成．
*
* Return  : なし．
*
*============================================================================*/
GWAVERESOURCE
GWaveResNew
(
	GSOUND			gsnd			/* サウンドハンドラ         */
)
{
	waveres*		wres;

	wres = new_object( sizeof ( waveres ) );

	wres->gsnd = gsnd;
	wres->n    = 0;
	wres->wave = NULL;

	return	(GWAVERESOURCE)wres;
}

/*=============================================================================
*
* Purpose : ＷＡＶＥリソースの消滅．
*
* Return  : なし．
*
*============================================================================*/
void
GWaveResDelete
(
	GWAVERESOURCE	hnd				/* ＷＡＶＥリソースハンドラ */
)
{
	waveres*		wres = (waveres*)hnd;

	/* すべてのＷＡＶＥリソースを解放 */
	GWaveResDelAllResource( hnd );

	del_object( wres );
}

/*=============================================================================
*
* Purpose : ＷＡＶＥリソースの初期化．
*
* Return  : なし．
*
*============================================================================*/
void
GWaveResInit
(
	GWAVERESOURCE	hnd,			/* ＷＡＶＥリソースハンドラ */
	int				n				/* ＷＡＶＥリソース数       */
)
{
	waveres*		wres = (waveres*)hnd;
	int				i;

	/* すべてのＷＡＶＥリソースを解放 */
	GWaveResDelAllResource( hnd );

	wres->n    = n;
	wres->wave = NULL;

	if ( wres->n != 0 )
	{
		wres->wave = new_object( sizeof( GWAVE* ) * wres->n );
	}
	for ( i = 0; i < wres->n; i++ )
	{
		wres->wave[i] = NULL;
	}
}

/*=============================================================================
*
* Purpose : ＷＡＶＥリソースをすべて解放．
*
* Return  : なし．
*
*============================================================================*/
void
GWaveResDelAllResource
(
	GWAVERESOURCE	hnd				/* ＷＡＶＥリソースハンドラ */
)
{
	waveres*		wres = (waveres*)hnd;
	int				i;

	if ( wres->wave == NULL )
	{
		return;
	}

	for ( i = 0; i < wres->n; i++ )
	{
		if ( wres->wave[i] != NULL )
		{
			GSoundDeleteWave( wres->gsnd, wres->wave[i] );
		
			wres->wave[i] = NULL;
		}
	}
	del_object( wres->wave );

	wres->n    = 0;
	wres->wave = NULL;
}

/*=============================================================================
*
* Purpose : ＷＡＶＥリソースの読み込み．
*
* Return  : 読み込みに成功したら真，失敗したら偽を返す．
*
*============================================================================*/
BOOL
GWaveResLoadResource
(
	GWAVERESOURCE	hnd,			/* ＷＡＶＥリソースハンドラ */
	int				id,				/* ＷＡＶＥリソースＩＤ     */
	const char*		wave_fname,		/* ＷＡＶＥファイル名       */
	int				cnt,			/* 同時再生数               */
	GWAVEFLAG		flag			/* 再生タイプフラグ         */
)
{
	waveres*		wres = (waveres*)hnd;
	GWAVE			wave;

	if ( id >= wres->n )
	{
		return	FALSE;
	}

	if ( wres->wave[ id ] != NULL )
	{
		return	FALSE;
	}

	/* ＷＡＶＥファイルの読み込み */
	if ( ( wave = GSoundLoadWave( wres->gsnd, wave_fname, cnt, flag ) ) == NULL )
	{
		return	FALSE;
	}
	wres->wave[ id ]= wave;

	return	TRUE;
}

/*=============================================================================
*
* Purpose : ＷＡＶＥリソースの取得．
*
* Return  : 読み込みに成功したら真，失敗したら偽を返す．
*
*============================================================================*/
GWAVE
GWaveResGetResource
(
	GWAVERESOURCE	hnd,			/* ＷＡＶＥリソースハンドラ */
	int				id				/* ＷＡＶＥリソースＩＤ     */
)
{
	waveres*		wres = (waveres*)hnd;

	if ( id >= wres->n )
	{
		return	NULL;
	}

	return	wres->wave[ id ];
}

/*=============================================================================
*
* Purpose : ＷＡＶＥリソースの解放．
*
* Return  : なし．
*
*============================================================================*/
void
GWaveResDelResource
(
	GWAVERESOURCE	hnd,			/* ＷＡＶＥリソースハンドラ */
	int				id				/* ＷＡＶＥリソースＩＤ     */
)
{
	waveres*		wres = (waveres*)hnd;

	if ( id >= wres->n )
	{
		return;
	}
	if ( wres->wave[ id ] == NULL )
	{
		return;
	}

	GSoundDeleteWave( wres->gsnd, wres->wave[ id ] );

	wres->wave[ id ] = NULL;
}

/********** End of File ******************************************************/
