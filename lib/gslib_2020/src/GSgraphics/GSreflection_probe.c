/******************************************************************************
* File Name : GSreflection_probe.c               Ver : 1.00  Date : 2020-08-13
*
* Description :
*
*       ���t���N�V�����v���[�u�D
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

/****** �f�[�^�^�錾 *********************************************************/

#define GL_REFRECTION_PROBE_BOX_PROJECTION  (1 << 0)

/*----- ���t���N�V�����v���[�u�\���� ----------------------------------------*/

typedef	struct                      /* ���t���N�V�����v���[�u���^	*/
{
    GSuint      dwFlag;		        /* ���t���N�V�����v���[�u�t���O */
    GSvector3   position;           /* ���t���N�V�����v���[�u�̈ʒu */
    GSvector3   boundsMin;          /* ���E�{�b�N�X�̍ŏ��l         */
    GSvector3   boundsMax;          /* ���E�{�b�N�X�̍ő�l         */
    GStexture*  pCubemap;			/* �L���[�u�}�b�v�e�N�X�`��     */
    GSuint      resolution;         /* �L���[�u�}�b�v�̉𑜓x       */
    GSuint      mipmapCount;        /* �~�b�v�}�b�v��               */

} GSreflectionProbeInfo;

typedef	struct						/* ���t���N�V�����v���[�u�^		*/
{
    /* �f�[�^�t���O */
    GSuint                 dwFlag;
    /* �V�[���S�̂̃��t���N�V�����v���[�u */
    GSreflectionProbeInfo* pEnviromemtReflection;
    /* ���t���N�V�����v���[�u��	    */
    GSuint			       numReflectionProbes;
    /* ���t���N�V�����v���[�u�z��	*/
    GSreflectionProbeInfo* pReflectionProbes;

}  GSreflectionProbe;

/****** �֐��v���g�^�C�v�錾 *************************************************/

static GSreflectionProbe* gsReflectionProbeNew(void);
static void gsReflectionProbeDelete(GSreflectionProbe* pReflectionProbe);
static GSboolean gsReflectionProbeLoad(GSreflectionProbe* pReflectionProbe, const char* pszFileName);
static void gsReflectionProbeBindTexture(GSreflectionProbe*  pReflectionProbe, const GSvector3* pPosition, const GSvector3* pSize);
static GStexture* LoadCubemapTexture(GFILE	file);
extern void gsSetShaderParam1f(const char*, GSfloat);
extern void gsSetShaderParamTexture(const char*, GSint);

/* ���t���N�V�����v���[�u�̍ő吔�@*/
#define GS_REFRECTION_PROBE_MAX	256

/* ���t���N�V�����v���[�u�f�[�^ */
static GSreflectionProbe*	s_pReflectionProbe[GS_REFRECTION_PROBE_MAX];

/* �o�C���h���̃��t���N�V�����v���[�u */
static GSuint			    s_BindReflectionProbe = 0;

/* �̈攻��p�̃o�E���f�B���O�{�b�N�X�̃T�C�Y */
static GSvector3            s_BoundingBoxSize = { 0.5f, 0.5f, 0.5f };

/*=============================================================================
*
* Purpose : ���t���N�V�����v���[�u�̓ǂݍ��݁D
*
* Return  :  �ǂݍ��݂���������ΐ^�A���s����΋U��Ԃ��D
*
*============================================================================*/
extern GSboolean
gsLoadReflectionProbe
(
    GSuint      id,			/* ���t���N�V�����v���[�uID            */
    const char* pszFileName	/* ���t���N�V�����v���[�u�t�@�C����    */
)
{
    GSreflectionProbe*		pReflectionProbe;
    /* ���t���N�V�����v���[�u�I�u�W�F�N�g�𐶐� */
    pReflectionProbe = gsReflectionProbeNew();
    /* ���t���N�V�����v���[�u�̓ǂݍ��� */
    if (gsReflectionProbeLoad(pReflectionProbe, pszFileName) == GS_FALSE) {
        gsReflectionProbeDelete(pReflectionProbe);
        return GS_FALSE;
    }
    /* �Â��f�[�^����ꍇ�͍폜 */
    if (s_pReflectionProbe[id] != NULL) {
        gsReflectionProbeDelete(s_pReflectionProbe[id]);
    }
    s_pReflectionProbe[id] = pReflectionProbe;
    return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : ���t���N�V�����v���[�u�̍폜�D
*
* Return  : �Ȃ��D
*
*============================================================================*/
extern void
gsDeleteReflectionProbe
(
    GSuint      id			/* ���t���N�V�����v���[�uID         */
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
* Purpose : �S���t���N�V�����v���[�u���폜�D
*
* Return  : �Ȃ��D
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
* Purpose : ���t���N�V�����v���[�u�̃o�C���h�D
*
* Return  : �Ȃ��D
*
*============================================================================*/
extern void
gsBindReflectionProbe
(
    GSuint      id			/* ���t���N�V�����v���[�uID         */
)
{
    s_BindReflectionProbe = id;
}

/*=============================================================================
*
* Purpose : ���t���N�V�����v���[�u�̃A���o�C���h
*
* Return  : �Ȃ��D
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
* Purpose : ���t���N�V�����v���[�u�p�̃e�N�X�`�����o�C���h�D
*
* Return  : �Ȃ��D
*
*============================================================================*/
extern void
gsBindReflectionProbeTexture
(
    const GSvector3*    pPosition       /* �o�E���f�B���O�{�b�N�X�̍��W    */
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
* Purpose : ���t���N�V�����v���[�u�p�̃e�N�X�`�����o�C���h�D
*
* Return  : �Ȃ��D
*
*============================================================================*/
extern void
gsBindReflectionProbeTextureEx
(
    const GSvector3* pPosition,     /* �o�E���f�B���O�{�b�N�X�̍��W    */
    const GSvector3* pSize          /* �o�E���f�B���O�{�b�N�X�̃T�C�Y  */
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
* Purpose : ���}�b�v�p�̃e�N�X�`�����o�C���h�D
*
* Return  : �Ȃ��D
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
* Purpose : ���t���N�V�����v���[�u�p�̃o�E���f�B���O�{�b�N�X�̃T�C�Y��ݒ�D
*
* Return  : �Ȃ��D
*
*============================================================================*/
extern void
gsSetReflectionProbeBoundigBoxSize
(
    const GSvector3*    pSize           /* �o�E���f�B���O�{�b�N�X�̃T�C�Y  */
)
{
    s_BoundingBoxSize = *pSize;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ���t���N�V�����v���[�u�I�u�W�F�N�g�̐���
*
* Return  : ���t���N�V�����v���[�u�I�u�W�F�N�g��Ԃ��D
*
*----------------------------------------------------------------------------*/
static GSreflectionProbe*
gsReflectionProbeNew
(
    void
)
{
    GSreflectionProbe* pReflectionProbe;
    // ���t���N�V�����v���[�u�I�u�W�F�N�g�𐶐�
    pReflectionProbe = new_object(sizeof(GSreflectionProbe));
    pReflectionProbe->numReflectionProbes = 0;
    pReflectionProbe->pEnviromemtReflection = NULL;
    pReflectionProbe->pReflectionProbes = NULL;
    return pReflectionProbe;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ���t���N�V�����v���[�u�I�u�W�F�N�g�̍폜
*
* Return  :  �Ȃ��D
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
* Purpose : ���t���N�V�����v���[�u�I�u�W�F�N�g�̓ǂݍ���
*
* Return  : �ǂݍ��݂���������ΐ^�A���s����΋U��Ԃ��D
*
*----------------------------------------------------------------------------*/
static GSboolean
gsReflectionProbeLoad
(
    GSreflectionProbe* pReflectionProbe,		/* ���t���N�V�����v���[�u			*/
    const char* pszFileName		                /* ���t���N�V�����v���[�u�t�@�C����	*/
)
{
    GFILE			file;
    GSuint			i;
    GSboolean       result;
    /* ���t���N�V�����v���[�u�t�@�C�����I�[�v������ */
    if ((file = GFileOpen(pszFileName, "r")) == NULL)
    {
        /* �t�@�C�����I�[�v���ł��Ȃ����� */
        return	GS_FALSE;
    }
    /* �Â��f�[�^���폜 */
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
 
    /* �I�v�V�����t���O��ǂݍ��� */
    GFileReadDwordHex(file, &pReflectionProbe->dwFlag);
    if ((pReflectionProbe->dwFlag & 0x00000001) != 0)
    {
        /* �V�[���S�̂̃��t���N�V�����v���[�u��ǂݍ��� */
        static const GSvector3 ZERO = { 0.0f, 0.0f, 0.0f };
        pReflectionProbe->pEnviromemtReflection = new_object(sizeof(GSreflectionProbeInfo));
        pReflectionProbe->pEnviromemtReflection->dwFlag = 0;
        pReflectionProbe->pEnviromemtReflection->boundsMin = ZERO;
        pReflectionProbe->pEnviromemtReflection->boundsMax = ZERO;
        /* �e�N�X�`���̉𑜓x��ǂݍ��� */
        GFileReadDword(file, &pReflectionProbe->pEnviromemtReflection->resolution);
        /* �e�N�X�`���̃~�b�v�}�b�v����ǂݍ��� */
        GFileReadDword(file, &pReflectionProbe->pEnviromemtReflection->mipmapCount);
        /* �L���[�u�}�b�v�̓ǂݍ��� */
        pReflectionProbe->pEnviromemtReflection->pCubemap = LoadCubemapTexture(file);
    }
    /* ���t���N�V�����v���[�u�̓ǂݍ��� */
    if ((pReflectionProbe->dwFlag & 0x00000002) != 0)
    {
        /* ���t���N�V�����v���[�u����ǂݍ��� */
        GFileReadDword(file, &pReflectionProbe->numReflectionProbes);
        // ���t���N�V�����v���[�u�z����m��
        pReflectionProbe->pReflectionProbes = new_object(sizeof(GSreflectionProbeInfo) * pReflectionProbe->numReflectionProbes);
        for (i = 0; i < pReflectionProbe->numReflectionProbes; ++i)
        {
            /* �t���O�̓ǂݍ��� */
            GFileReadDword(file, &pReflectionProbe->pReflectionProbes[i].dwFlag);
            /* ���t���N�V�����v���[�u�̈ʒu�̓ǂݍ��� */
            GFileReadVector3(file, &pReflectionProbe->pReflectionProbes[i].position);
            /* ���E�{�b�N�X�̓ǂݍ��� */
            GFileReadVector3(file, &pReflectionProbe->pReflectionProbes[i].boundsMin);
            GFileReadVector3(file, &pReflectionProbe->pReflectionProbes[i].boundsMax);
            /* �e�N�X�`���̉𑜓x��ǂݍ��� */
            GFileReadDword(file, &pReflectionProbe->pReflectionProbes[i].resolution);
            /* �e�N�X�`���̃~�b�v�}�b�v����ǂݍ��� */
            GFileReadDword(file, &pReflectionProbe->pReflectionProbes[i].mipmapCount);
            /* �L���[�u�}�b�v�̓ǂݍ��� */
            pReflectionProbe->pReflectionProbes[i].pCubemap = LoadCubemapTexture(file);
        }
    }
    // �G���[�`�F�b�N
    result = GFileIsError(file) == GS_FALSE;
    GFileClose(file);
    return result;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ���t���N�V�����v���[�u�I�u�W�F�N�g�̃o�C���h
*
* Return  : �Ȃ��D
*
*----------------------------------------------------------------------------*/
static void
gsReflectionProbeBindTexture
(
    GSreflectionProbe*  pReflectionProbe,		/* ���t���N�V�����v���[�u		  */
    const GSvector3*    pPosition,              /* �o�E���f�B���O�{�b�N�X�̍��W   */
    const GSvector3*    pSize                   /* �o�E���f�B���O�{�b�N�X�̃T�C�Y */
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

    /* ���t���N�V�����v���[�u���Ȃ��ꍇ�܂��͍��W�̎w�肪�Ȃ��ꍇ */
    if (pReflectionProbe->numReflectionProbes == 0 || pPosition == NULL)
    {
        /* �f�t�H���g�̃L���[�u�}�b�v���o�C���h���� */
        if (pReflectionProbe->pEnviromemtReflection != NULL)
        {
            /* �V�F�[�_�[�p�̃p�����[�^�ݒ� */
            gsSetShaderParam1f("gs_SpecularCubemapBlendLerp", 0.0f);
            gsSetShaderParamTexture("gs_SpecularCubemap0", 13);
            gsSetShaderParam1f("gs_SpecularCubemap0_BoxProjection", 0.0f);
            gsSetShaderParam1f("gs_SpecularCubemap0_MipmapCount", pReflectionProbe->pEnviromemtReflection->mipmapCount);
            gsSetShaderParamTexture("gs_SpecularCubemap1", 14);
            gsSetShaderParam1f("gs_SpecularCubemap1_BoxProjection", 0.0f);
            gsSetShaderParam1f("gs_SpecularCubemap1_MipmapCount", pReflectionProbe->pEnviromemtReflection->mipmapCount);
            /* �L���[�u�}�b�v�e�N�X�`���̃o�C���h */
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
    /* �o�E���f�B���O�{�b�N�X�̍쐬 */
    boundsMin.x = pPosition->x - size.x;
    boundsMin.y = pPosition->y - size.y;
    boundsMin.z = pPosition->z - size.z;
    boundsMax.x = pPosition->x + size.x;
    boundsMax.y = pPosition->y + size.y;
    boundsMax.z = pPosition->z + size.z;

    /* �u�����h�l�̏����� */
    blendLerp = 0.0f;

    /* �̐ς��傫�����ɂQ�̃��t���N�V�����v���[�u��T�� */
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
                // 2�Ԗڂ��X�V
                maxVolumeIndex1 = maxVolumeIndex0;
                maxVolume1 = maxVolume0;
                // �ő�l���X�V
                maxVolumeIndex0 = i;
                maxVolume0 = volume;
            }
            else if (volume > maxVolume1)
            {
                // 2�Ԗڂ��X�V
                maxVolumeIndex1 = i;
                maxVolume1 = volume;
            }
        }
    }
    /* �̐ς̍��v�����߂� */
    totalVolume = maxVolume0 + maxVolume1;
    if (totalVolume > 0.0f)
    {
        /* �̐ϔ���u�����h�l�Ƃ��Ďg�p���� */
        blendLerp = 1.0f - (maxVolume0 / totalVolume);
    }
    else if (pReflectionProbe->numReflectionProbes >= 2)
    {
        /* �̐ς̍��v���O�̏ꍇ�͍ł��߂����t���N�V�����v���[�u��I������ */
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
    // �L���[�u�}�b�v�̃u�����h�l
    gsSetShaderParam1f("gs_SpeculerCubemapBlendLerp", blendLerp);
    /* �L���[�u�}�b�v�O */
    pInfo0 = &pReflectionProbe->pReflectionProbes[maxVolumeIndex0];
    gsSetShaderParam1f("gs_SpecularCubemap0_BoxProjection", (pInfo0->dwFlag & GL_REFRECTION_PROBE_BOX_PROJECTION) ? 1.0f: 0.0f);
    gsSetShaderParam3f("gs_SpecularCubemap0_Position", &pInfo0->position);
    gsSetShaderParam3f("gs_SpecularCubemap0_BoxMin", &pInfo0->boundsMin);
    gsSetShaderParam3f("gs_SpecularCubemap0_BoxMax", &pInfo0->boundsMax);
    gsSetShaderParam1f("gs_SpecularCubemap0_MipmapCount", pInfo0->mipmapCount);
    gsSetShaderParamTexture("gs_SpecularCubemap0", 13);

    /* �L���[�u�}�b�v�P */
    pInfo1 = &pReflectionProbe->pReflectionProbes[maxVolumeIndex1];
    gsSetShaderParam1f("gs_SpecularCubemap1_BoxProjection", (pInfo1->dwFlag & GL_REFRECTION_PROBE_BOX_PROJECTION) ? 1.0f : 0.0f);
    gsSetShaderParam3f("gs_SpecularCubemap1_Position", &pInfo1->position);
    gsSetShaderParam3f("gs_SpecularCubemap1_BoxMin", &pInfo1->boundsMin);
    gsSetShaderParam3f("gs_SpecularCubemap1_BoxMax", &pInfo1->boundsMax);
    gsSetShaderParam1f("gs_SpecularCubemap1_MipmapCount", pInfo0->mipmapCount);
    gsSetShaderParamTexture("gs_SpecularCubemap1", 14);

    /* �L���[�u�}�b�v�e�N�X�`���̃o�C���h */
    glActiveTexture(GL_TEXTURE14);
    gsTextureBind(pInfo1->pCubemap);
    glActiveTexture(GL_TEXTURE13);
    gsTextureBind(pInfo0->pCubemap);
    glActiveTexture(GL_TEXTURE0);
}

/*-----------------------------------------------------------------------------
*
* Purpose : �e�N�X�`���̓ǂݍ��݁D
*
* Return  : �Ȃ��D
*
*----------------------------------------------------------------------------*/
static GStexture*
LoadCubemapTexture
(
    GFILE		file					/* �t�@�C��			*/
)
{
    char		TexFileName[_MAX_PATH];
    char		Drive[_MAX_DRIVE];
    char		Dir[_MAX_DIR];
    GStexture*  pCubemap;

    /* �I�[�v�����Ă���t�@�C���̃p�X���擾���� */
    gsSplitPath(GFileGetFileName(file), Drive, Dir, NULL, NULL);
    strcpy(TexFileName, Drive);
    strcat(TexFileName, Dir);

    /* �e�N�X�`���t�@�C�����̓ǂݍ��� */
    GFileReadString(file, TexFileName + strlen(TexFileName));

    /* �e�N�X�`���𐶐� */
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
