//=============================================================================
//
// �V�[������ [scene.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCENE_UIOBJECT_MAX		(10)
#define SCENE_GAME_WIN			(50.0f)		// ��������

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class BaseScene
{
public:
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual ~BaseScene() {};
};

class SceneManager
{
public:
	// �V�[�����X�g
	enum SCENE 	{TITLE, GAME, RESULT, MAX	};
	// �R���X�g���N�^
	SceneManager(void);
	// �f�X�g���N�^
	~SceneManager(void);
	// �V�[���������ɂ���ĕς���֐�
	static void			ChangeScene(SCENE scene);
	// �V�[���̏������֐�
	static void			Init(HINSTANCE hInst, HWND hWnd);
	// �V�[���̏I���֐�
	static void			Uninit(void);
	// �V�[���̍X�V�֐�
	static void			Update(void);
	// �V�[���̕`��֐�
	static void			Draw(void);

	// ���݂̃V�[���擾
	static SCENE		GetScene(void)				{ return m_eScene; }

	// �c�������ݒ�
	static void			SetRate(float fRate)		{ m_fRate = fRate; }
	// �c�������擾
	static float		GetRate(void)				{ return m_fRate; }
	// �c�������ݒ�i�{�[�i�X���݁j
	static void			SetRateInBonus(float fRateInBonus)	{ m_fRateInBonus = fRateInBonus; }
	// �c�������擾�i�{�[�i�X���݁j
	static float		GetRateInBonus(void)				{ return m_fRateInBonus; }

	// �����v���C���[�ݒ�
	static void			SetWin(int nWin)			{ m_nWin = nWin; }
	// �����v���C���[�擾
	static int			GetWin(void)				{ return m_nWin; }

	// AI�̋����ݒ�
	static void			SetAiLevel(int nAiLevel)	{ m_nAiLevel = nAiLevel; }
	// AI�̋����擾
	static int			GetAiLevel(void)			{ return m_nAiLevel; }

	// �v���C���[�l���ݒ�
	static void			SetPlayerNumS(int nNum)		{ m_nPlayerNum = nNum; }
	// �v���C���[�l���擾
	static int			GetPlayerNumS(void)			{ return m_nPlayerNum; }
	// �v���C���[�^�C�v�ݒ�
	static void			SetPlayerTypeS(int nType)	{ m_nPlayerType = nType; }
	// �v���C���[�^�C�v�擾
	static int			GetPlayerTypeS(void)		{ return m_nPlayerType; }
	// SE�̏I���֐�
	static void			EndSeS(void);

	static int			m_nTotalBurnBonus;
	static int			m_nTotalWetBonus;
	static int			m_nTotalBurnChain;

private:
	static BaseScene	*m_pScene;			// ���݂̃V�[��
	static SCENE		m_eScene;			// ���݂̃V�[���ԍ�
	static float		m_fRate;			// �c������
	static float		m_fRateInBonus;		// �c�������i�{�[�i�X���݁j
	static int			m_nWin;				// �����v���C���[
	static int			m_nAiLevel;			// AI�̋���
	static int			m_nPlayerNum;		// �v���C���[�l��
	static int			m_nPlayerType;		// �v���C���[�^�C�v


#ifdef _DEBUG
	// �f�o�b�O�p�V�[���J��
	static void DebugScene(void);
#endif
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

#endif