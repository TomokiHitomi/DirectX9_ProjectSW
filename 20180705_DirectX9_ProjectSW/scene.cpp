//=============================================================================
//
// �V�[������ [scene.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "scene.h"

/* Scene */
#include "title.h"
#include "game.h"
#include "result.h"

/* System */
#include "camera.h"
#include "light.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

/* Ui */

/* Object */

/* �f�o�b�O */
#ifdef _DEBUG
#include "debugproc.h"
#endif

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//�n�߂̓V�[������ɂ��Ă���
BaseScene *SceneManager::m_pScene = NULL;

SceneManager::SCENE	SceneManager::m_eScene;
float				SceneManager::m_fRate;
float				SceneManager::m_fRateInBonus;
int					SceneManager::m_nWin;
int					SceneManager::m_nAiLevel;
int					SceneManager::m_nPlayerNum;
int					SceneManager::m_nPlayerType;

int					SceneManager::m_nTotalBurnBonus;
int					SceneManager::m_nTotalWetBonus;
int					SceneManager::m_nTotalBurnChain;

//=============================================================================
// �V�[���Ǘ�����
//=============================================================================
void SceneManager::ChangeScene(SCENE scene)
{

	if (m_pScene != NULL)
	{
		delete m_pScene;
	}

	// SE�̒�~����
	EndSeS();

	// �I�����ꂽ�V�[����ۊ�
	m_eScene = scene;

	switch (scene)
	{	//�����̃V�[��
	case SCENE::TITLE:
		SetSoundBgm(SOUND_BGM_NON);
		m_pScene = new TitleScene();	// �^�C�g���V�[�������݂̃V�[���ɂ���
		break;
	case SCENE::GAME:
		SetSoundBgm(SOUND_BGM_NON);
		m_pScene = new GameScene();		// �Q�[���V�[�������݂̃V�[���ɂ���
		break;
	case SCENE::RESULT:
		SetSoundBgm(SOUND_BGM_NON);
		m_pScene = new ResultScene();	// ���U���g�V�[�������݂̃V�[���ɂ���
		break;
	default:
		break;
	}
}

//=============================================================================
// ����������
//=============================================================================
void SceneManager::Init(HINSTANCE hInst, HWND hWnd)
{
	InitLight();			// ���C�g
	InitInput(hInst, hWnd);	// ����
	InitSound(hWnd);		// �T�E���h
	InitFade();				// �t�F�[�h
	Camera::Init();			// �J����(�I�������̓V�[���}�l�[�W���[�̂݁j

	ChangeScene(m_eScene);	// �����V�[���ݒ�

#ifdef _DEBUG
	InitDebugProc();		// �f�o�b�O
#endif
}

//=============================================================================
// �I������
//=============================================================================
void SceneManager::Uninit(void)
{

	UninitInput();			// ����
	Camera::Uninit();			// �J����
	UninitSound();			// �T�E���h
	UninitFade();			// �t�F�[�h

#ifdef _DEBUG
	UninitDebugProc();		// �f�o�b�O
#endif
}

//=============================================================================
// �X�V����
//=============================================================================
void SceneManager::Update(void)
{
	UpdateInput();			// ����
	UpdateFade();			// �t�F�[�h

#ifdef _DEBUG
	DebugScene();			// �f�o�b�O�p
#endif

	m_pScene->Update();		// ���݂̃V�[���̍X�V�֐�

	Camera::Update();			// �J����
	UpdateSound();			// �T�E���h
}

//=============================================================================
// �`�揈��
//=============================================================================
void SceneManager::Draw(void)
{
	Camera::Set();			// �J����

	m_pScene->Draw();		// ���݂̃V�[���̕`��֐�

	DrawFade();				// �t�F�[�h

#ifdef _DEBUG
	DrawDebugProc();
#endif
}

//=============================================================================
// �R���X�g���N�^����
//=============================================================================
SceneManager::SceneManager(void)
{
	// �����̃V�[����TITLE�ɐݒ�
	m_eScene = TITLE;

	// �e�ϐ���������
	m_nWin = 0;				// �����v���C���[
	m_nAiLevel = 0;			// AI�̋���
	m_fRate = 0.0f;			// �c������
	m_fRateInBonus = 0.0f;	// �c�������i�{�[�i�X���݁j
	m_nPlayerNum = 0;		// �v���C���[�l��
	m_nPlayerType = 0;		// �v���C���[�^�C�v
}

//=============================================================================
// �f�X�g���N�^����
//=============================================================================
SceneManager::~SceneManager(void)
{
	if (m_pScene != NULL)
	{
		delete m_pScene;
	}
}

//=============================================================================
// �V�[���Ǘ�����
//=============================================================================
void SceneManager::EndSeS(void)
{
	EndSe(SE_FIRE1);
	EndSe(SE_WATER_SUCK);
	EndSe(SE_WATER1);
	EndSe(SE_WATER2);
	EndSe(SE_WIND_SUCK);
	EndSe(SE_WIND1);
}

#ifdef _DEBUG
//=============================================================================
// �f�o�b�O����
//=============================================================================
void SceneManager::DebugScene(void)
{
	// �f�o�b�O�̍X�V����
	UpdateDebugProc();

	// �V�[������\��
	PrintDebugProc("�y SCENE �z\n");

	// ���݂̃V�[����\��
	switch (m_eScene)
	{
	case SCENE::TITLE:
		PrintDebugProc("Scene[TITLE ]  ");
		break;
	case SCENE::GAME:
		PrintDebugProc("Scene[ GAME ]  ");
		break;
	case SCENE::RESULT:
		PrintDebugProc("Scene[RESULT]  ");
		break;
	}


	// �v���C���[�l���A�R���g���[���[�ԍ��A�c��������\��
	PrintDebugProc("Rate[%f]  1P:Num[%d]  Type[%d]\n",
		m_fRate, m_nPlayerNum, m_nPlayerType);

	//PrintDebugProc("�y Result �z\n");
	//PrintDebugProc("Rate [%f]  ", GetStageRate());
	//switch (GetStageWinPlayer())
	//{
	//case STAGE_WIN_NON:
	//	PrintDebugProc("Win[NON]\n");
	//	break;
	//case STAGE_WIN_1P:
	//	PrintDebugProc("Win[1P]\n");
	//	break;
	//case STAGE_WIN_2P:
	//	PrintDebugProc("Win[2P]\n");
	//	break;
	//}
	//PrintDebugProc("\n");
	if (GetKeyboardTrigger(DIK_F2))
	{
		SetFadeScene(TITLE);
		//ChangeScene(TITLE);
	}
	if (GetKeyboardTrigger(DIK_F3))
	{
		SetFadeScene(GAME);
		//ChangeScene(RESULT);
	}
	if (GetKeyboardTrigger(DIK_F4))
	{
		SetFadeScene(RESULT);
		//ChangeScene(SELECT);
	}
	// SE�e�X�g�isound.h��include���Ċ֐����Ăԁj
	//if (GetKeyboardTrigger(DIK_1))
	//{	// 
	//	SetSe(SE_SELECT_ERROR, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}
	//if (GetKeyboardTrigger(DIK_2))
	//{	// 
	//	SetSe(SE_GAME_SELECT, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}
	//if (GetKeyboardTrigger(DIK_3))
	//{	// 
	//	SetSe(SE_GAME_KETTEI, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}
	//if (GetKeyboardTrigger(DIK_4))
	//{	//		
	//	SetSe(SE_RESULT_SELECT, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}
	//if (GetKeyboardTrigger(DIK_5))
	//{	// 
	//	SetSe(SE_RESULT_KETTEI, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}
	//if (GetKeyboardTrigger(DIK_6))
	//{	// 
	//	SetVoice(VOICE_3, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}
	//if (GetKeyboardTrigger(DIK_7))
	//{	// 
	//	SetVoice(VOICE_2, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}
	//if (GetKeyboardTrigger(DIK_8))
	//{	// 
	//	SetVoice(VOICE_START, E_DS8_FLAG_NONE, SOUND_OPTION_CONTINUE_ON, 0);
	//}

	PrintDebugProc("\n");
}
#endif