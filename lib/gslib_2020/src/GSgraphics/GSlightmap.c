/******************************************************************************
* File Name : GSlightmap.c                       Ver : 1.00  Date : 2018-08-21
*
* Description :
*
*       ライトマップ．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSlightmap.h"
#include	"GSload_texture_file.h"
#include	"GSfile_path.h"
#include	"heap.h"
#include	"gfile.h"
#include	<string.h>
#include	<opengl_ext.h>

/****** データ型宣言 *********************************************************/

#define GS_LIGHTMAP_COLOR		(1 << 0)
#define GS_LIGHTMAP_DIRECTION	(1 << 1)
#define GS_LIGHTMAP_SHADOW_MASK	(1 << 2)
#define GS_LIGHTMAP_HDR			(1 << 3)

/*----- ライトマップ構造体 --------------------------------------------------*/

typedef	struct							/* ライトマップ情報型			*/
{
	GSuint     dwFlag;					/* ライトマップフラグ           */
	GStexture* pLightmapColor;			/* ライトマップカラーテクスチャ */
	GStexture* pLightmapDirection;		/* ライトマップ方向テクスチャ   */
	GStexture* pShadowMask;				/* シャドウマップテクスチャ     */
	GSfloat	   fLightmapHDR;			/* ライトマップHDR              */

} GSlightmapInfo;

typedef	struct							/* ライトマップ型				*/
{
	GSuint			numLightmaps;		/* ライトマップ数				*/
	GSlightmapInfo* pLightmaps;			/* ライトマップ配列				*/
} GSlightmap;

/****** 関数プロトタイプ宣言 *************************************************/

static GSlightmap* gsLightmapNew(void);
static void gsLightmapDelete(GSlightmap* pLightmap);
static GSboolean gsLightmapLoad(GSlightmap* pLightmap, const char* pszFileName);
static void gsLightmapBind(GSlightmap* pLightmap, int index);
static GStexture* LoadLightmapTexture(GFILE	file);
extern void gsSetShaderParam1f(const char*, GSfloat);
extern void gsSetShaderParamTexture(const char*, GSint);

/* ライトマップの最大数　*/
#define GS_LIGHTMAP_MAX	256

/* ライトマップデータ */
static GSlightmap*		s_pLightmaps[GS_LIGHTMAP_MAX];

/* バインド中のライトマップ */
static GSuint			s_BindLightmap = 0;

/*=============================================================================
*
* Purpose : ライトマップの読み込み
*
* Return  : 読み込みが成功すれば真、失敗すれば偽を返す
*
*============================================================================*/
extern GSboolean
gsLoadLightmap
(
	GSuint      id,			/* ライトマップID         */
	const char* pszFileName	/* ライトマップファイル名 */
)
{
	GSlightmap*		pLightmap;
	/* ライトマップオブジェクトを生成 */
	pLightmap = gsLightmapNew();
	/* ライトマップオブジェクトの読み込み */
	if (gsLightmapLoad(pLightmap, pszFileName) == GS_FALSE) {
		gsLightmapDelete(pLightmap);
		return GS_FALSE;
	}
	/* 古いデータある場合は削除 */
	if (s_pLightmaps[id] != NULL) {
		gsLightmapDelete(s_pLightmaps[id]);
	}
	s_pLightmaps[id] = pLightmap;
	return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : ライトマップの削除．
*
* Return  : なし
*
*============================================================================*/
extern void
gsDeleteLightmap
(
	GSuint      id			/* ライトマップID         */
)
{
	if (id >= GS_LIGHTMAP_MAX)
	{
		return;
	}
	if (s_pLightmaps[id] != NULL)
	{
		gsLightmapDelete(s_pLightmaps[id]);
		s_pLightmaps[id] = NULL;
	}
}

/*=============================================================================
*
* Purpose : 全ライトマップを削除
*
* Return  : なし
*
*============================================================================*/
extern void
gsCleanUpLightmap
(
	void
)
{
	int id;
	for (id = 0; id < GS_LIGHTMAP_MAX; ++id) {
		gsDeleteLightmap(id);
	}
}

/*=============================================================================
*
* Purpose : ライトマップのバインド
*
* Return  : なし
*
*============================================================================*/
extern void
gsBindLightmap
(
	GSuint      id			/* ライトマップID         */
)
{
	s_BindLightmap = id;
}

/*=============================================================================
*
* Purpose : ライトマップオブジェクトのアンバインド
*
* Return  : なし
*
*============================================================================*/
extern void
gsUnBindLightmap
(
	void
)
{
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}

/*=============================================================================
*
* Purpose : ライトマップ用のテクスチャをバインド
*
* Return  : なし
*
*============================================================================*/
extern void
gsBindLightmapTexture
(
	GSuint      index		/* ライトマップインデックス */
)
{
	if (s_pLightmaps[s_BindLightmap] == NULL)
	{
		return;
	}
	gsLightmapBind(s_pLightmaps[s_BindLightmap], index);
}

/*-----------------------------------------------------------------------------
*
* Purpose : ライトマップオブジェクトの生成
*
* Return  : ライトマップオブジェクトを返す
*
*----------------------------------------------------------------------------*/
static GSlightmap*
gsLightmapNew
(
	void
)
{
	GSlightmap* pLightMap;
	// ライトマップオブジェクトを生成
	pLightMap = new_object(sizeof(GSlightmap));
	pLightMap->numLightmaps = 0;
	pLightMap->pLightmaps = NULL;
	return pLightMap;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ライトマップオブジェクトの削除
*
* Return  : なし
*
*----------------------------------------------------------------------------*/
static void
gsLightmapDelete
(
	GSlightmap* pLightmap
)
{
	GSuint i;
	for (i = 0; i < pLightmap->numLightmaps; ++i)
	{
		if (pLightmap->pLightmaps[i].pLightmapColor != NULL)
		{
			gsTextureDelete(pLightmap->pLightmaps[i].pLightmapColor);
		}
		if (pLightmap->pLightmaps[i].pLightmapDirection != NULL)
		{
			gsTextureDelete(pLightmap->pLightmaps[i].pLightmapDirection);
		}
		if (pLightmap->pLightmaps[i].pShadowMask != NULL)
		{
			gsTextureDelete(pLightmap->pLightmaps[i].pShadowMask);
		}
	}
	del_object(pLightmap->pLightmaps);
	del_object(pLightmap);
}

/*-----------------------------------------------------------------------------
*
* Purpose : ライトマップオブジェクトの読み込み
*
* Return  : 読み込みが成功すれば真、失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
gsLightmapLoad
(
	GSlightmap* pLightmap,		/* ライトマップ				*/
	const char* pszFileName		/* ライトマップファイル名	*/
)
{
	GFILE			file;
	GSuint			i;
	GSboolean       result;
	/* ライトマップファイルをオープンする */
	if ((file = GFileOpen(pszFileName, "r")) == NULL)
	{
		/* ファイルがオープンできなかった */
		return	GS_FALSE;
	}
	/* ライトマップ数を読み込む */
	GFileReadDword(file, &pLightmap->numLightmaps);
	/* 古いデータを削除 */
	if (pLightmap->pLightmaps != NULL)
	{
		del_object(pLightmap->pLightmaps);
	}
	// ライトマップ配列を確保
	pLightmap->pLightmaps = new_object(sizeof(GSlightmapInfo) * pLightmap->numLightmaps);
	for (i = 0; i < pLightmap->numLightmaps; ++i)
	{
		/* ライトマップテクスチャフラグの読み込み */
		GFileReadDword(file, &pLightmap->pLightmaps[i].dwFlag);
		// ライトマップカラーの読み込み
		pLightmap->pLightmaps[i].pLightmapColor = NULL;
		if ((pLightmap->pLightmaps[i].dwFlag & GS_LIGHTMAP_COLOR) != 0)
		{
			pLightmap->pLightmaps[i].pLightmapColor = LoadLightmapTexture(file);
		}
		// ライトマップディレクションの読み込み
		pLightmap->pLightmaps[i].pLightmapDirection = NULL;
		if ((pLightmap->pLightmaps[i].dwFlag & GS_LIGHTMAP_DIRECTION) != 0)
		{
			pLightmap->pLightmaps[i].pLightmapDirection = LoadLightmapTexture(file);
		}
		// シャドウマスクの読み込み
		pLightmap->pLightmaps[i].pShadowMask = NULL;
		if ((pLightmap->pLightmaps[i].dwFlag & GS_LIGHTMAP_SHADOW_MASK) != 0)
		{
			pLightmap->pLightmaps[i].pShadowMask = LoadLightmapTexture(file);
		}
		pLightmap->pLightmaps[i].fLightmapHDR = 5.0f;
		if ((pLightmap->pLightmaps[i].dwFlag & GS_LIGHTMAP_HDR) != 0)
		{
			// ライトマップHDRの読み込み
			GFileReadFloat(file, &pLightmap->pLightmaps[i].fLightmapHDR);
		}
	}
	// エラーチェック
	result = GFileIsError(file) == GS_FALSE;
	GFileClose(file);
	return result;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ライトマップオブジェクトのバインド
*
* Return  : 読み込みが成功すれば真、失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static void
gsLightmapBind
(
	GSlightmap* pLightmap,		/* ライトマップ				*/
	int			index			/* インデックス				*/
)
{
	gsSetShaderParam1f("gs_LightmapHDR", pLightmap->pLightmaps[index].fLightmapHDR);
    gsSetShaderParamTexture("gs_LigtmapColor", 10);
    gsSetShaderParamTexture("gs_LigtmapDirection", 11);
    gsSetShaderParamTexture("gs_LigtmapShadowMask", 12);

	if (pLightmap->pLightmaps[index].pLightmapColor != NULL)
	{
		glActiveTexture(GL_TEXTURE10);
		gsTextureBind(pLightmap->pLightmaps[index].pLightmapColor);
	}
	if (pLightmap->pLightmaps[index].pLightmapDirection != NULL)
	{
		glActiveTexture(GL_TEXTURE11);
		gsTextureBind(pLightmap->pLightmaps[index].pLightmapDirection);
	}
	if (pLightmap->pLightmaps[index].pShadowMask != NULL)
	{
		glActiveTexture(GL_TEXTURE12);
		gsTextureBind(pLightmap->pLightmaps[index].pShadowMask);
	}
	glActiveTexture(GL_TEXTURE0);
}

/*-----------------------------------------------------------------------------
*
* Purpose : テクスチャの読み込み．
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static GStexture*
LoadLightmapTexture
(
	GFILE		file					/* ファイル			*/
)
{
	char		TexFileName[_MAX_PATH];
	char		Drive[_MAX_DRIVE];
	char		Dir[_MAX_DIR];
	GStexture*	textute;

	/* オープンしているメッシュファイルのパスを取得する */
	gsSplitPath(GFileGetFileName(file), Drive, Dir, NULL, NULL);
	strcpy(TexFileName, Drive);
	strcat(TexFileName, Dir);

	/* テクスチャファイル名の読み込み */
	GFileReadString(file, TexFileName + strlen(TexFileName));

	/* テクスチャを生成 */
	textute = gsLoadTextureFile(TexFileName);

	gsTextureBind(textute);
	if (textute->Mipmap > 0) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	gsTextureUnbind(textute);

	return textute;
}

/********** End of File ******************************************************/
