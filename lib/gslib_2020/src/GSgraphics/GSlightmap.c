/******************************************************************************
* File Name : GSlightmap.c                       Ver : 1.00  Date : 2018-08-21
*
* Description :
*
*       ���C�g�}�b�v�D
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

/****** �f�[�^�^�錾 *********************************************************/

#define GS_LIGHTMAP_COLOR		(1 << 0)
#define GS_LIGHTMAP_DIRECTION	(1 << 1)
#define GS_LIGHTMAP_SHADOW_MASK	(1 << 2)
#define GS_LIGHTMAP_HDR			(1 << 3)

/*----- ���C�g�}�b�v�\���� --------------------------------------------------*/

typedef	struct							/* ���C�g�}�b�v���^			*/
{
	GSuint     dwFlag;					/* ���C�g�}�b�v�t���O           */
	GStexture* pLightmapColor;			/* ���C�g�}�b�v�J���[�e�N�X�`�� */
	GStexture* pLightmapDirection;		/* ���C�g�}�b�v�����e�N�X�`��   */
	GStexture* pShadowMask;				/* �V���h�E�}�b�v�e�N�X�`��     */
	GSfloat	   fLightmapHDR;			/* ���C�g�}�b�vHDR              */

} GSlightmapInfo;

typedef	struct							/* ���C�g�}�b�v�^				*/
{
	GSuint			numLightmaps;		/* ���C�g�}�b�v��				*/
	GSlightmapInfo* pLightmaps;			/* ���C�g�}�b�v�z��				*/
} GSlightmap;

/****** �֐��v���g�^�C�v�錾 *************************************************/

static GSlightmap* gsLightmapNew(void);
static void gsLightmapDelete(GSlightmap* pLightmap);
static GSboolean gsLightmapLoad(GSlightmap* pLightmap, const char* pszFileName);
static void gsLightmapBind(GSlightmap* pLightmap, int index);
static GStexture* LoadLightmapTexture(GFILE	file);
extern void gsSetShaderParam1f(const char*, GSfloat);
extern void gsSetShaderParamTexture(const char*, GSint);

/* ���C�g�}�b�v�̍ő吔�@*/
#define GS_LIGHTMAP_MAX	256

/* ���C�g�}�b�v�f�[�^ */
static GSlightmap*		s_pLightmaps[GS_LIGHTMAP_MAX];

/* �o�C���h���̃��C�g�}�b�v */
static GSuint			s_BindLightmap = 0;

/*=============================================================================
*
* Purpose : ���C�g�}�b�v�̓ǂݍ���
*
* Return  : �ǂݍ��݂���������ΐ^�A���s����΋U��Ԃ�
*
*============================================================================*/
extern GSboolean
gsLoadLightmap
(
	GSuint      id,			/* ���C�g�}�b�vID         */
	const char* pszFileName	/* ���C�g�}�b�v�t�@�C���� */
)
{
	GSlightmap*		pLightmap;
	/* ���C�g�}�b�v�I�u�W�F�N�g�𐶐� */
	pLightmap = gsLightmapNew();
	/* ���C�g�}�b�v�I�u�W�F�N�g�̓ǂݍ��� */
	if (gsLightmapLoad(pLightmap, pszFileName) == GS_FALSE) {
		gsLightmapDelete(pLightmap);
		return GS_FALSE;
	}
	/* �Â��f�[�^����ꍇ�͍폜 */
	if (s_pLightmaps[id] != NULL) {
		gsLightmapDelete(s_pLightmaps[id]);
	}
	s_pLightmaps[id] = pLightmap;
	return	GS_TRUE;
}

/*=============================================================================
*
* Purpose : ���C�g�}�b�v�̍폜�D
*
* Return  : �Ȃ�
*
*============================================================================*/
extern void
gsDeleteLightmap
(
	GSuint      id			/* ���C�g�}�b�vID         */
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
* Purpose : �S���C�g�}�b�v���폜
*
* Return  : �Ȃ�
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
* Purpose : ���C�g�}�b�v�̃o�C���h
*
* Return  : �Ȃ�
*
*============================================================================*/
extern void
gsBindLightmap
(
	GSuint      id			/* ���C�g�}�b�vID         */
)
{
	s_BindLightmap = id;
}

/*=============================================================================
*
* Purpose : ���C�g�}�b�v�I�u�W�F�N�g�̃A���o�C���h
*
* Return  : �Ȃ�
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
* Purpose : ���C�g�}�b�v�p�̃e�N�X�`�����o�C���h
*
* Return  : �Ȃ�
*
*============================================================================*/
extern void
gsBindLightmapTexture
(
	GSuint      index		/* ���C�g�}�b�v�C���f�b�N�X */
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
* Purpose : ���C�g�}�b�v�I�u�W�F�N�g�̐���
*
* Return  : ���C�g�}�b�v�I�u�W�F�N�g��Ԃ�
*
*----------------------------------------------------------------------------*/
static GSlightmap*
gsLightmapNew
(
	void
)
{
	GSlightmap* pLightMap;
	// ���C�g�}�b�v�I�u�W�F�N�g�𐶐�
	pLightMap = new_object(sizeof(GSlightmap));
	pLightMap->numLightmaps = 0;
	pLightMap->pLightmaps = NULL;
	return pLightMap;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ���C�g�}�b�v�I�u�W�F�N�g�̍폜
*
* Return  : �Ȃ�
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
* Purpose : ���C�g�}�b�v�I�u�W�F�N�g�̓ǂݍ���
*
* Return  : �ǂݍ��݂���������ΐ^�A���s����΋U��Ԃ��D
*
*----------------------------------------------------------------------------*/
static GSboolean
gsLightmapLoad
(
	GSlightmap* pLightmap,		/* ���C�g�}�b�v				*/
	const char* pszFileName		/* ���C�g�}�b�v�t�@�C����	*/
)
{
	GFILE			file;
	GSuint			i;
	GSboolean       result;
	/* ���C�g�}�b�v�t�@�C�����I�[�v������ */
	if ((file = GFileOpen(pszFileName, "r")) == NULL)
	{
		/* �t�@�C�����I�[�v���ł��Ȃ����� */
		return	GS_FALSE;
	}
	/* ���C�g�}�b�v����ǂݍ��� */
	GFileReadDword(file, &pLightmap->numLightmaps);
	/* �Â��f�[�^���폜 */
	if (pLightmap->pLightmaps != NULL)
	{
		del_object(pLightmap->pLightmaps);
	}
	// ���C�g�}�b�v�z����m��
	pLightmap->pLightmaps = new_object(sizeof(GSlightmapInfo) * pLightmap->numLightmaps);
	for (i = 0; i < pLightmap->numLightmaps; ++i)
	{
		/* ���C�g�}�b�v�e�N�X�`���t���O�̓ǂݍ��� */
		GFileReadDword(file, &pLightmap->pLightmaps[i].dwFlag);
		// ���C�g�}�b�v�J���[�̓ǂݍ���
		pLightmap->pLightmaps[i].pLightmapColor = NULL;
		if ((pLightmap->pLightmaps[i].dwFlag & GS_LIGHTMAP_COLOR) != 0)
		{
			pLightmap->pLightmaps[i].pLightmapColor = LoadLightmapTexture(file);
		}
		// ���C�g�}�b�v�f�B���N�V�����̓ǂݍ���
		pLightmap->pLightmaps[i].pLightmapDirection = NULL;
		if ((pLightmap->pLightmaps[i].dwFlag & GS_LIGHTMAP_DIRECTION) != 0)
		{
			pLightmap->pLightmaps[i].pLightmapDirection = LoadLightmapTexture(file);
		}
		// �V���h�E�}�X�N�̓ǂݍ���
		pLightmap->pLightmaps[i].pShadowMask = NULL;
		if ((pLightmap->pLightmaps[i].dwFlag & GS_LIGHTMAP_SHADOW_MASK) != 0)
		{
			pLightmap->pLightmaps[i].pShadowMask = LoadLightmapTexture(file);
		}
		pLightmap->pLightmaps[i].fLightmapHDR = 5.0f;
		if ((pLightmap->pLightmaps[i].dwFlag & GS_LIGHTMAP_HDR) != 0)
		{
			// ���C�g�}�b�vHDR�̓ǂݍ���
			GFileReadFloat(file, &pLightmap->pLightmaps[i].fLightmapHDR);
		}
	}
	// �G���[�`�F�b�N
	result = GFileIsError(file) == GS_FALSE;
	GFileClose(file);
	return result;
}

/*-----------------------------------------------------------------------------
*
* Purpose : ���C�g�}�b�v�I�u�W�F�N�g�̃o�C���h
*
* Return  : �ǂݍ��݂���������ΐ^�A���s����΋U��Ԃ��D
*
*----------------------------------------------------------------------------*/
static void
gsLightmapBind
(
	GSlightmap* pLightmap,		/* ���C�g�}�b�v				*/
	int			index			/* �C���f�b�N�X				*/
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
* Purpose : �e�N�X�`���̓ǂݍ��݁D
*
* Return  : �Ȃ��D
*
*----------------------------------------------------------------------------*/
static GStexture*
LoadLightmapTexture
(
	GFILE		file					/* �t�@�C��			*/
)
{
	char		TexFileName[_MAX_PATH];
	char		Drive[_MAX_DRIVE];
	char		Dir[_MAX_DIR];
	GStexture*	textute;

	/* �I�[�v�����Ă��郁�b�V���t�@�C���̃p�X���擾���� */
	gsSplitPath(GFileGetFileName(file), Drive, Dir, NULL, NULL);
	strcpy(TexFileName, Drive);
	strcat(TexFileName, Dir);

	/* �e�N�X�`���t�@�C�����̓ǂݍ��� */
	GFileReadString(file, TexFileName + strlen(TexFileName));

	/* �e�N�X�`���𐶐� */
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
