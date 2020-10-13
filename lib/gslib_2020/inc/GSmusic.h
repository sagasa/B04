#ifndef _GS_MUSIC_H_
#define _GS_MUSIC_H_

#include <gstype.h>

#ifdef __cplusplus
extern "C" {
#endif

// ���y�̏�����
void gsInitMusic(void);

// �E�B���h���A�N�e�B�u�̏ꍇ�̏���
void gsActivateMusic(GSboolean active);

// ���y�̏I������
void gsFinishMusic(void);

// �N���[���A�b�v
void gsCleanupMusic(void);

// �ǂݍ���
GSboolean gsLoadMusic(GSuint musicID, const char* fileName, GSboolean loop);

// ���y�̃o�C���h
void gsBindMusic(GSuint musicID);

// �폜
void gsDeleteMusic(GSuint musicID);

// �Đ�
void gsPlayMusic(void);

// �ꎞ��~
void gsPauseMusic(void);

// ���X�^�[�g
void gsRestartMusic(void);

// ��~
void gsStopMusic(void);

// �Đ������H
GSboolean gsIsPlayMusic(void);

// �|�[�Y�����H
GSboolean gsIsPauseMusic(void);

// ���[�v�Đ����Ă��邩
GSboolean gsIsLoopMusic(void);

// �Đ��I�����Ă��邩�H
GSboolean gsIsEndMusic(void);

// �{�����[���̐ݒ�
void gsSetMusicVolume(GSfloat volume);

// �o�����X�̐ݒ�
void gsSetMusicBalance(GSfloat balance);

// �Đ��^�C�}�̐ݒ�
void gsSetMusicTime(float time);

// ���݂̍Đ����Ԃ̎擾
GSfloat gsGetMusicTime(void);

// �I�����Ԃ̎擾
GSfloat gsGetMusicEndTime(void);

// �{�����[���̎擾
GSfloat gsGetMusicVolume(void);

// �o�����X�̎擾
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
