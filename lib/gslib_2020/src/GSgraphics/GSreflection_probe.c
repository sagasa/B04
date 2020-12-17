/******************************************************************************
* File Name : GSreflection_probe.c               Ver : 1.00  Date : 2020-08-13
*
* Description :
*
*       リフレクションプローブ．
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#include	"GSreflection_probe.h"
#include	"GSload_texture_file.h"
#include	"GSfile_path.h"
#include	"heap.h"
#include	"gfile.h"
#include	<string.h>
#include    <float.h>
#include	<opengl_ext.h>

/****** データ型宣言 *********************************************************/

#define GL_REFRECTION_PROBE_BOX_PROJECTION  (1 << 0)

/*----- リフレクションプローブ構造体 ----------------------------------------*/

typedef	struct                      /* リフレクションプローブ情報型	*/
{
    GSuint      dwFlag;		        /* リフレクションプローブフラグ */
    GSvector3   position;           /* リフレクションプローブの位置 */
    GSvector3   boundsMin;          /* 境界ボックスの最小値         */
    GSvector3   boundsMax;          /* 境界ボックスの最大値         */
    GStexture*  pCubemap;			/* キューブマップテクスチャ     */
    GSuint      resolution;         /* キューブマップの解像度       */
    GSuint      mipmapCount;        /* ミップマップ数               */

} GSreflectionProbeInfo;

typedef	struct						/* リフレクションプローブ型		*/
{
    /* データフラグ */
    GSuint                 dwFlag;
    /* シーン全体のリフレクションプローブ */
    GSreflectionProbeInfo* pEnviromemtReflection;
    /* リフレクションプローブ数	    */
    GSuint			       numReflectionProbes;
    /* リフレクションプローブ配列	*/
    GSreflectionProbeInfo* pReflectionProbes;

}  GSreflectionProbe;

/****** 関数プロトタイプ宣言 *************************************************/

static GSreflectionProbe* gsReflectionProbeNew(void);
static void gsReflectionProbeDelete(GSreflectionProbe* pReflectionProbe);
static GSboolean gsReflectionProbeLoad(GSreflectionProbe* pReflectionProbe, const char* pszFileName);
static void gsReflectionProbeBindTexture(GSreflectionProbe*  pReflectionProbe, const GSvector3* pPosition, const GSvector3* pSize);
static GStexture* LoadCubemapTexture(GFILE	file);
extern void gsSetShaderParam1f(const char*, GSfloat);
extern void gsSetShaderParamTexture(const char*, GSint);

/* リフレクションプローブの最大数　*/
#define GS_REFRECTION_PROBE_MAX	256

/* リフレクションプローブデータ */
static GSreflectionProbe*	s_pReflectionProbe[GS_REFRECTION_PROBE_MAX];

/* バインド中のリフレクションプローブ */
static GSuint			    s_BindReflectionProbe = 0;

/* 領域判定用のバウンディングボックスのサイズ */
static GSvector3            s_BoundingBoxSize = { 0.5f, 0.5f, 0.5f };

/*=============================================================================
*
* Purpose : リフレクションプローブの読み込み．
*
* Return  :  読み込みが成功すれば真、失敗すれば偽を返す．
*
*============================================================================*/
extern GSboolean
gsLoadReflectionProbe
(
    GSuint      id,			/* リフレクションプローブID            */
    const char* pszFileName	/* リフレクションプローブファイル名    */
)
{
    GSreflectionProbe*		pReflectionProbe;
    /* リフレクションプローブオブジェクトを生成 */
    pReflectionProbe = gsReflectionProbeNew();
    /* リフレクションプローブの読み込み */
    if (gsReflectionProbeLoad(pReflectionProbe, pszFileName) == GS_FALSE) {
        gsReflectionProbeDelete(pReflectionProbe);
        return GS_FALSE;
    }
    /* 古いデータある場合は削除 */
    if (s_pReflectionProbe[id] != NULL) {
        gsReflectionProbeDelete(s_pReflectionProbe[id]);
    }
    s_pReflectionProbe[id] = pReflectionProbe;
    return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : リフレクションプローブの削除．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsDeleteReflectionProbe
(
    GSuint      id			/* リフレクションプローブID         */
)
{
    if (id >= GS_REFRECTION_PROBE_MAX)
    {
        return;
    }
    if (s_pReflectionProbe[id] != NULL)
    {
        gsReflectionProbeDelete(s_pReflectionProbe[id]);
        s_pReflectionProbe[id] = NULL;
    }
}

/*=============================================================================
*
* Purpose : 全リフレクションプローブを削除．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsCleanUpReflectionProbe
(
    void
)
{
    int id;
    for (id = 0; id < GS_REFRECTION_PROBE_MAX; ++id) {
        gsDeleteReflectionProbe(id);
    }
}

/*=============================================================================
*
* Purpose : リフレクションプローブのバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBindReflectionProbe
(
    GSuint      id			/* リフレクションプローブID         */
)
{
    s_BindReflectionProbe = id;
}

/*=============================================================================
*
* Purpose : リフレクションプローブのアンバインド
*
* Return  : なし．
*
*============================================================================*/
extern void
gsUnBindReflectionProb
(
    void
)
{
    glActiveTexture(GL_TEXTURE13);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE14);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
}

/*=============================================================================
*
* Purpose : リフレクションプローブ用のテクスチャをバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBindReflectionProbeTexture
(
    const GSvector3*    pPosition       /* バウンディングボックスの座標    */
)
{
    if (s_pReflectionProbe[s_BindReflectionProbe] == NULL)
    {
        return;
    }
    gsReflectionProbeBindTexture(s_pReflectionProbe[s_BindReflectionProbe], pPosition, &s_BoundingBoxSize);
}

/*=============================================================================
*
* Purpose : リフレクションプローブ用のテクスチャをバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBindReflectionProbeTextureEx
(
    const GSvector3* pPosition,     /* バウンディングボックスの座標    */
    const GSvector3* pSize          /* バウンディングボックスのサイズ  */
)
{
    if (s_pReflectionProbe[s_BindReflectionProbe] == NULL)
    {
        return;
    }
    gsReflectionProbeBindTexture(s_pReflectionProbe[s_BindReflectionProbe], pPosition, pSize);
}

/*=============================================================================
*
* Purpose : 環境マップ用のテクスチャをバインド．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsBindEnvironmentReflectionTexture
(
    void
)
{
    if (s_pReflectionProbe[s_BindReflectionProbe] == NULL)
    {
        return;
    }
    gsReflectionProbeBindTexture(s_pReflectionProbe[s_BindReflectionProbe], NULL, NULL);
}

/*=============================================================================
*
* Purpose : リフレクションプローブ用のバウンディングボックスのサイズを設定．
*
* Return  : なし．
*
*============================================================================*/
extern void
gsSetReflectionProbeBoundigBoxSize
(
    const GSvector3*    pSize           /* バウンディングボックスのサイズ  */
)
{
    s_BoundingBoxSize = *pSize;
}

/*-----------------------------------------------------------------------------
*
* Purpose : リフレクションプローブオブジェクトの生成
*
* Return  : リフレクションプローブオブジェクトを返す．
*
*----------------------------------------------------------------------------*/
static GSreflectionProbe*
gsReflectionProbeNew
(
    void
)
{
    GSreflectionProbe* pReflectionProbe;
    // リフレクションプローブオブジェクトを生成
    pReflectionProbe = new_object(sizeof(GSreflectionProbe));
    pReflectionProbe->numReflectionProbes = 0;
    pReflectionProbe->pEnviromemtReflection = NULL;
    pReflectionProbe->pReflectionProbes = NULL;
    return pReflectionProbe;
}

/*-----------------------------------------------------------------------------
*
* Purpose : リフレクションプローブオブジェクトの削除
*
* Return  :  なし．
*
*----------------------------------------------------------------------------*/
static void
gsReflectionProbeDelete
(
    GSreflectionProbe* pReflectionProbe
)
{
    GSuint i;
    for (i = 0; i < pReflectionProbe->numReflectionProbes; ++i)
    {
        if (pReflectionProbe->pReflectionProbes[i].pCubemap != NULL)
        {
            gsTextureDelete(pReflectionProbe->pReflectionProbes[i].pCubemap);
        }
    }
    del_object(pReflectionProbe->pReflectionProbes);
    if (pReflectionProbe->pEnviromemtReflection != NULL)
    {
        gsTextureDelete(pReflectionProbe->pEnviromemtReflection->pCubemap);
        del_object(pReflectionProbe->pEnviromemtReflection);
    }
    del_object(pReflectionProbe);
}

/*-----------------------------------------------------------------------------
*
* Purpose : リフレクションプローブオブジェクトの読み込み
*
* Return  : 読み込みが成功すれば真、失敗すれば偽を返す．
*
*----------------------------------------------------------------------------*/
static GSboolean
gsReflectionProbeLoad
(
    GSreflectionProbe* pReflectionProbe,		/* リフレクションプローブ			*/
    const char* pszFileName		                /* リフレクションプローブファイル名	*/
)
{
    GFILE			file;
    GSuint			i;
    GSboolean       result;
    /* リフレクションプローブファイルをオープンする */
    if ((file = GFileOpen(pszFileName, "r")) == NULL)
    {
        /* ファイルがオープンできなかった */
        return	GS_FALSE;
    }
    /* 古いデータを削除 */
    if (pReflectionProbe->pEnviromemtReflection != NULL)
    {
        del_object(pReflectionProbe->pEnviromemtReflection);
        pReflectionProbe->pEnviromemtReflection = NULL;
    }
    if (pReflectionProbe->pReflectionProbes != NULL)
    {
        del_object(pReflectionProbe->pReflectionProbes);
        pReflectionProbe->pReflectionProbes = NULL;
    }
 
    /* オプションフラグを読み込む */
    GFileReadDwordHex(file, &pReflectionProbe->dwFlag);
    if ((pReflectionProbe->dwFlag & 0x00000001) != 0)
    {
        /* シーン全体のリフレクションプローブを読み込む */
        static const GSvector3 ZERO = { 0.0f, 0.0f, 0.0f };
        pReflectionProbe->pEnviromemtReflection = new_object(sizeof(GSreflectionProbeInfo));
        pReflectionProbe->pEnviromemtReflection->dwFlag = 0;
        pReflectionProbe->pEnviromemtReflection->boundsMin = ZERO;
        pReflectionProbe->pEnviromemtReflection->boundsMax = ZERO;
        /* テクスチャの解像度を読み込み */
        GFileReadDword(file, &pReflectionProbe->pEnviromemtReflection->resolution);
        /* テクスチャのミップマップ数を読み込み */
        GFileReadDword(file, &pReflectionProbe->pEnviromemtReflection->mipmapCount);
        /* キューブマップの読み込み */
        pReflectionProbe->pEnviromemtReflection->pCubemap = LoadCubemapTexture(file);
    }
    /* リフレクションプローブの読み込み */
    if ((pReflectionProbe->dwFlag & 0x00000002) != 0)
    {
        /* リフレクションプローブ数を読み込む */
        GFileReadDword(file, &pReflectionProbe->numReflectionProbes);
        // リフレクションプローブ配列を確保
        pReflectionProbe->pReflectionProbes = new_object(sizeof(GSreflectionProbeInfo) * pReflectionProbe->numReflectionProbes);
        for (i = 0; i < pReflectionProbe->numReflectionProbes; ++i)
        {
            /* フラグの読み込み */
            GFileReadDword(file, &pReflectionProbe->pReflectionProbes[i].dwFlag);
            /* リフレクションプローブの位置の読み込み */
            GFileReadVector3(file, &pReflectionProbe->pReflectionProbes[i].position);
            /* 境界ボックスの読み込み */
            GFileReadVector3(file, &pReflectionProbe->pReflectionProbes[i].boundsMin);
            GFileReadVector3(file, &pReflectionProbe->pReflectionProbes[i].boundsMax);
            /* テクスチャの解像度を読み込み */
            GFileReadDword(file, &pReflectionProbe->pReflectionProbes[i].resolution);
            /* テクスチャのミップマップ数を読み込み */
            GFileReadDword(file, &pReflectionProbe->pReflectionProbes[i].mipmapCount);
            /* キューブマップの読み込み */
            pReflectionProbe->pReflectionProbes[i].pCubemap = LoadCubemapTexture(file);
        }
    }
    // エラーチェック
    result = GFileIsError(file) == GS_FALSE;
    GFileClose(file);
    return result;
}

/*-----------------------------------------------------------------------------
*
* Purpose : リフレクションプローブオブジェクトのバインド
*
* Return  : なし．
*
*----------------------------------------------------------------------------*/
static void
gsReflectionProbeBindTexture
(
    GSreflectionProbe*  pReflectionProbe,		/* リフレクションプローブ		  */
    const GSvector3*    pPosition,              /* バウンディングボックスの座標   */
    const GSvector3*    pSize                   /* バウンディングボックスのサイズ */
)
{
    GSvector3               boundsMin;
    GSvector3               boundsMax;
    GSvector3               volumeBoundsMin;
    GSvector3               volumeBoundsMax;
    GSfloat                 maxVolume0;
    GSuint                  maxVolumeIndex0;
    GSfloat                 maxVolume1;
    GSuint                  maxVolumeIndex1;
    GSreflectionProbeInfo*  pInfo;
    GSfloat                 totalVolume;
    GSfloat                 blendLerp;
    GSuint                  i;
    GSfloat                 width;
    GSfloat                 height;
    GSfloat                 depth;
    GSfloat                 volume;
    GSvector3               size = { 0.0f, 0.0f, 0.0f };
    GSreflectionProbeInfo*  pInfo0;
    GSreflectionProbeInfo*  pInfo1;

    /* リフレクションプローブがない場合または座標の指定がない場合 */
    if (pReflectionProbe->numReflectionProbes == 0 || pPosition == NULL)
    {
        /* デフォルトのキューブマップをバインドする */
        if (pReflectionProbe->pEnviromemtReflection != NULL)
        {
            /* シェーダー用のパラメータ設定 */
            gsSetShaderParam1f("gs_SpecularCubemapBlendLerp", 0.0f);
            gsSetShaderParamTexture("gs_SpecularCubemap0", 13);
            gsSetShaderParam1f("gs_SpecularCubemap0_BoxProjection", 0.0f);
            gsSetShaderParam1f("gs_SpecularCubemap0_MipmapCount", pReflectionProbe->pEnviromemtReflection->mipmapCount);
            gsSetShaderParamTexture("gs_SpecularCubemap1", 14);
            gsSetShaderParam1f("gs_SpecularCubemap1_BoxProjection", 0.0f);
            gsSetShaderParam1f("gs_SpecularCubemap1_MipmapCount", pReflectionProbe->pEnviromemtReflection->mipmapCount);
            /* キューブマップテクスチャのバインド */
            glActiveTexture(GL_TEXTURE13);
            gsTextureBind(pReflectionProbe->pEnviromemtReflection->pCubemap);
            glActiveTexture(GL_TEXTURE14);
            gsTextureBind(pReflectionProbe->pEnviromemtReflection->pCubemap);
            glActiveTexture(GL_TEXTURE0);
        }
        return;
    }
    if (pSize == NULL) {
        size = *pSize;
    }
    /* バウンディングボックスの作成 */
    boundsMin.x = pPosition->x - size.x;
    boundsMin.y = pPosition->y - size.y;
    boundsMin.z = pPosition->z - size.z;
    boundsMax.x = pPosition->x + size.x;
    boundsMax.y = pPosition->y + size.y;
    boundsMax.z = pPosition->z + size.z;

    /* ブレンド値の初期化 */
    blendLerp = 0.0f;

    /* 体積が大きい順に２つのリフレクションプローブを探す */
    maxVolume0 = 0.0f;
    maxVolumeIndex0 = 0;
    maxVolume1 = 0.0f;
    maxVolumeIndex1 = 0;
    if ((size.x * size.y * size.z) > 0.0f)
    {
        for (i = 0; i < pReflectionProbe->numReflectionProbes; ++i)
        {
            pInfo = &pReflectionProbe->pReflectionProbes[i];
            volumeBoundsMin.x = CLAMP(boundsMin.x, pInfo->boundsMin.x, pInfo->boundsMax.x);
            volumeBoundsMin.y = CLAMP(boundsMin.y, pInfo->boundsMin.y, pInfo->boundsMax.y);
            volumeBoundsMin.z = CLAMP(boundsMin.z, pInfo->boundsMin.z, pInfo->boundsMax.z);
            volumeBoundsMax.x = CLAMP(boundsMax.x, pInfo->boundsMin.x, pInfo->boundsMax.x);
            volumeBoundsMax.y = CLAMP(boundsMax.y, pInfo->boundsMin.y, pInfo->boundsMax.y);
            volumeBoundsMax.z = CLAMP(boundsMax.z, pInfo->boundsMin.z, pInfo->boundsMax.z);
            width = volumeBoundsMax.x - volumeBoundsMin.x;
            height = volumeBoundsMax.y - volumeBoundsMin.y;
            depth = volumeBoundsMax.z - volumeBoundsMin.z;
            volume = width * height * depth;
            if (volume > maxVolume0)
            {
                // 2番目を更新
                maxVolumeIndex1 = maxVolumeIndex0;
                maxVolume1 = maxVolume0;
                // 最大値を更新
                maxVolumeIndex0 = i;
                maxVolume0 = volume;
            }
            else if (volume > maxVolume1)
            {
                // 2番目を更新
                maxVolumeIndex1 = i;
                maxVolume1 = volume;
            }
        }
    }
    /* 体積の合計を求める */
    totalVolume = maxVolume0 + maxVolume1;
    if (totalVolume > 0.0f)
    {
        /* 体積比をブレンド値として使用する */
        blendLerp = 1.0f - (maxVolume0 / totalVolume);
    }
    else if (pReflectionProbe->numReflectionProbes >= 2)
    {
        /* 体積の合計が０の場合は最も近いリフレクションプローブを選択する */
        GSint minDistanceIndex = 0;
        GSfloat minDistance = FLT_MAX;
        for (i = 0; i < pReflectionProbe->numReflectionProbes; ++i)
        {
            pInfo = &pReflectionProbe->pReflectionProbes[i];
            GSvector3 nearest;
            nearest.x = CLAMP(pPosition->x, pInfo->boundsMin.x, pInfo->boundsMax.x);
            nearest.y = CLAMP(pPosition->y, pInfo->boundsMin.y, pInfo->boundsMax.y);
            nearest.z = CLAMP(pPosition->z, pInfo->boundsMin.z, pInfo->boundsMax.z);
            float distance = gsVector3Distance(pPosition, &nearest);
            if (distance < minDistance) {
                minDistanceIndex = i;
                minDistance = distance;
            }
        }
        maxVolumeIndex0 = minDistanceIndex;
        maxVolumeIndex1 = minDistanceIndex;
        blendLerp = 0.0f;
    }
    // キューブマップのブレンド値
    gsSetShaderParam1f("gs_SpeculerCubemapBlendLerp", blendLerp);
    /* キューブマップ０ */
    pInfo0 = &pReflectionProbe->pReflectionProbes[maxVolumeIndex0];
    gsSetShaderParam1f("gs_SpecularCubemap0_BoxProjection", (pInfo0->dwFlag & GL_REFRECTION_PROBE_BOX_PROJECTION) ? 1.0f: 0.0f);
    gsSetShaderParam3f("gs_SpecularCubemap0_Position", &pInfo0->position);
    gsSetShaderParam3f("gs_SpecularCubemap0_BoxMin", &pInfo0->boundsMin);
    gsSetShaderParam3f("gs_SpecularCubemap0_BoxMax", &pInfo0->boundsMax);
    gsSetShaderParam1f("gs_SpecularCubemap0_MipmapCount", pInfo0->mipmapCount);
    gsSetShaderParamTexture("gs_SpecularCubemap0", 13);

    /* キューブマップ１ */
    pInfo1 = &pReflectionProbe->pReflectionProbes[maxVolumeIndex1];
    gsSetShaderParam1f("gs_SpecularCubemap1_BoxProjection", (pInfo1->dwFlag & GL_REFRECTION_PROBE_BOX_PROJECTION) ? 1.0f : 0.0f);
    gsSetShaderParam3f("gs_SpecularCubemap1_Position", &pInfo1->position);
    gsSetShaderParam3f("gs_SpecularCubemap1_BoxMin", &pInfo1->boundsMin);
    gsSetShaderParam3f("gs_SpecularCubemap1_BoxMax", &pInfo1->boundsMax);
    gsSetShaderParam1f("gs_SpecularCubemap1_MipmapCount", pInfo0->mipmapCount);
    gsSetShaderParamTexture("gs_SpecularCubemap1", 14);

    /* キューブマップテクスチャのバインド */
    glActiveTexture(GL_TEXTURE14);
    gsTextureBind(pInfo1->pCubemap);
    glActiveTexture(GL_TEXTURE13);
    gsTextureBind(pInfo0->pCubemap);
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
LoadCubemapTexture
(
    GFILE		file					/* ファイル			*/
)
{
    char		TexFileName[_MAX_PATH];
    char		Drive[_MAX_DRIVE];
    char		Dir[_MAX_DIR];
    GStexture*  pCubemap;

    /* オープンしているファイルのパスを取得する */
    gsSplitPath(GFileGetFileName(file), Drive, Dir, NULL, NULL);
    strcpy(TexFileName, Drive);
    strcat(TexFileName, Dir);

    /* テクスチャファイル名の読み込み */
    GFileReadString(file, TexFileName + strlen(TexFileName));

    /* テクスチャを生成 */
    pCubemap = gsLoadTextureFile(TexFileName);

    gsTextureBind(pCubemap);
    glTexParameteri(pCubemap->Type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(pCubemap->Type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(pCubemap->Type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(pCubemap->Type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(pCubemap->Type, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    gsTextureUnbind(pCubemap);

    return pCubemap;
}

/********** End of File ******************************************************/
