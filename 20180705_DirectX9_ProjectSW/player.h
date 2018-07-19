//=============================================================================
//
// �v���C���[���� [player.h]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

/*******************************************************************************
* �C���N���[�h
*******************************************************************************/
#include "object.h"
#include "SkinMeshX.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
// ���f��
#define PLAYER_MODEL				("flower.X")

// ���f���X�P�[��
#define PLAYER_SCL					(0.5f)

#define PLAYER_MOVE_SPEED			(0.05f)
#define PLAYER_MOVE_SPEED_MAX		(10.0f)
#define PLAYER_MOVE_SPEED_MIN		(1.0f)

#define PLAYER_ROT_SPEED_X			(0.0005f)
#define PLAYER_ROT_SPEED_Z			(0.0015f)

#define PLAYER_ROT_SPEED_MAX_X		(0.02f)
#define PLAYER_ROT_SPEED_MAX_Z		(0.03f)

#define PLAYER_HEIGHT				(30.0f)
#define PLAYER_MOVE_INERTIA			(0.3f)
#define PLAYER_ROT_INERTIA			(0.3f)

#define PLAYER_ALPHA_TEST			(150)

/***** FloatMode *****/
#define PLAYER_FLOAT_ROT_X			(D3DX_PI * 0.5f)
#define PLAYER_FLOAT_ROT_Y			(D3DX_PI)

/***** FlyMode *****/

/***** ChangeMode *****/
#define PLAYER_CHANGE_FRAME			(10)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************

class Player : public Object
{
private:
	enum MODE
	{	// �v���C���[�̃��[�h�Ǘ�
		MODE_FLOAT,
		MODE_FLY,
		MODE_LOCKON,
		MODE_CHANGE,
		MODE_MAX
	};
	CSkinMesh		*m_CSkinMesh;		// �X�L�����b�V���i�[�p
	D3DXVECTOR3		m_vPos;				// ���W���
	D3DXVECTOR3		m_vRot;				// ��]���
	D3DXVECTOR3		m_vScl;				// �g�k���
	D3DXVECTOR3		m_vMove;			// �ړ��ʏ��
	D3DXVECTOR3		m_vRotIner;			// ��]���
	D3DXVECTOR3		m_vRotChange;		// ��]���
	D3DXVECTOR3		m_vUp;

	// ���[�J���x�N�g��
	D3DXVECTOR3		m_vX;
	D3DXVECTOR3		m_vY;
	D3DXVECTOR3		m_vZ;

	D3DXMATRIX		m_mtxWorld;			// ���[���h�}�g���N�X
	MODE			m_eMode;
	MODE			m_eModeNext;
	MODE			m_eModeOld;
	
	int				m_nCount;			// �ėp�J�E���^�[
	float			m_fMoveSpeed;
	bool			m_bUse;				// �g�p�t���O
public:
	enum PLAYER
	{	// �v���C���[�Ǘ�
		PLAYER_1P,
		PLAYER_MAX
	};

	// �R���X�g���N�^�i�����������j
	Player(void);
	//�f�X�g���N�^�i�I�������j
	~Player(void);

	// �X�V����
	void	Update(void);
	// �`�揈��
	void	Draw(void);
private:
	void	Float(void);
	void	Fly(void);
	void	Change(void);
	void	ModeChange(void);
	void	Move(void);
	void	MoveFunc(float);
	void	RotFunc(D3DXVECTOR3);
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

#endif
