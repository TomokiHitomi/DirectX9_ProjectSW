//=============================================================================
//
// ���͏��� [sound.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef ___SOUND_H___
#define ___SOUND_H___

#include <windows.h>
#include <tchar.h>
#include <dsound.h>
#include <mmsystem.h>

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define VOLUME_CONTROL_UP		(120)			// ���ʏ㏸��
#define VOLUME_CONTROL_DOWN		(20)			// ���ʉ��~��
#define VOLUME_MAX				(-500)			// �ő剹��
#define VOLUME_HALF				(-2000)			// ���ԉ���
#define VOLUME_MIN				(-8000)			// �ŏ�����
#define VOLUME_SE				(-800)			// SE�{�����[��
#define VOLUME_VOICE			(-500)			// �{�C�X�{�����[��

#define SE_11_CONTINUITY		(5)				// �q�b�gSE�̍Đ��Ԋu
#define SE_11_VOLUME_SE			(-1200)			// SE11�̃{�����[���ݒ�

#define SOUND_BGM_MAX			(SE_00)
#define SOUND_SE_MAX			(VOICE_00 - SE_00)
#define SOUND_VOICE_MAX			(SOUND_MAX - VOICE_00)

#define SOUND_SE_DRAMROLL_END	(1000000)

enum
{	// �T�E���h�ʂ��i���o�[(sound.cpp�̏��Ԃƍ��킹�鎖)
	BGM_00,
	BGM_01,
	BGM_02,
	BGM_03,
	BGM_04,
	SE_00,
	SE_01,
	SE_02,
	SE_03,
	SE_04,
	SE_05,
	SE_06,
	SE_07,
	SE_08,
	SE_09,
	SE_10,
	SE_11,
	SE_12,
	SE_13,
	SE_14,
	SE_15,
	SE_16,
	SE_17,
	SE_18,
	SE_19,
	SE_20,
	SE_21,
	SE_22,
	SE_23,
	SE_24,
	VOICE_00,
	VOICE_01,
	VOICE_02,
	VOICE_03,
	SOUND_MAX
};

enum
{	// BGM
	BGM_WHITELILY_V_NON,
	BGM_WHITELILY_V,
	BGM_RESULT,
	BGM_SELECT,
	BGM_RESULT2
};

enum
{	// SE
	SE_BUTTON,
	SE_BULLET,
	SE_MISS,
	SE_BOMB,
	SE_ITEM,
	SE_ENEMY_WALK,
	SE_WHISTLE_SHORT,
	SE_WHISTLE_NORMAL,
	SE_WHISTLE_CHAIN,
	SE_FIRE,
	SE_FIRE1,
	SE_WATER_SUCK,
	SE_WATER1,
	SE_WATER2,
	SE_WIND_SUCK,
	SE_WIND1,
	SE_DRAMROLL,
	SE_FIREEND,
	SE_WATEREND,
	SE_GAME_START,
	SE_GAME_SELECT,
	SE_GAME_KETTEI,
	SE_RESULT_SELECT,
	SE_RESULT_KETTEI,
	SE_SELECT_ERROR
};

enum
{	// VOICE
	VOICE_START,
	VOICE_1,
	VOICE_2,
	VOICE_3
};

enum
{	// �Đ��p�t���O
	E_DS8_FLAG_NONE,
	E_DS8_FLAG_LOOP,	// DSBPLAY_LOOPING=1
	E_DS8_FLAG_MAX
};

enum
{	// �Đ��p�t���O
	VOLUME_FADE_OUT,
	VOLUME_FADE_IN,
	VOLUME_FADE_HALF
};

enum
{	// �A���Đ��t���O
	CONTINUITY_OFF,
	CONTINUITY_ON
};

enum
{	// BGM�t���O
	SOUND_BGM_NON,
	SOUND_BGM_TITLE,
	SOUND_BGM_SELECT,
	SOUND_BGM_GAME,
	SOUND_BGM_RESULT,
	SOUND_BGM_TEST1,
	SOUND_BGM_TEST2
};

enum
{	// PlaySound��nOption�p
	SOUND_OPTION_CONTINUE_OFF,	// �A���Đ��I�t�i�A�Ŗ����j
	SOUND_OPTION_CONTINUE_ON,	// �A���Đ��I���i�A�őΉ��j
	SOUND_OPTION_BGM,			// BGM�Đ����Ɏg�p����
	SOUND_OPTION_CURRENT,		// �J�����g�|�W�V�����ύX�i�A�Ŗ����j
	SOUND_OPTION_MAX
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT					InitSound( HWND hWnd );	// ������
void					UninitSound();			// ��Еt��
void					UpdateSound(void);
LPDIRECTSOUNDBUFFER8	LoadSound( int no );	// �T�E���h�̃��[�h
void					PlaySound(LPDIRECTSOUNDBUFFER8 pBuffer, int flag/*=0*/, int nOption, DWORD dwPos);
//void					PlayContinuitySound(LPDIRECTSOUNDBUFFER8 pBuffer, int flag = 0 );
bool					IsPlaying( LPDIRECTSOUNDBUFFER8 pBuffer );					// �Đ������ǂ���
HRESULT					SetVol(LPDIRECTSOUNDBUFFER8 pBuffer, LONG lVol);
void					SetSoundBgm(int nBgm);
void					SetSoundBgmBoss(bool bBgm);
void					SetSe(int nSe, int nFlag, int nOption, DWORD dwPos);
void					EndSe(int nSe);
void					SetVoice(int nVoice, int nFlag, int nOption, DWORD dwPos);
#endif