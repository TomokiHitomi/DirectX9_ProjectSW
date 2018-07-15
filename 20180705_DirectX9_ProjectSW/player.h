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
#define PLAYER_MOVE_SPEED			(3.0f)

#define PLAYER_ROT_SPEED_X			(0.01f)
#define PLAYER_ROT_SPEED_Z			(0.03f)

#define PLAYER_HEIGHT				(30.0f)
#define PLAYER_MOVE_INERTIA			(0.3f)
#define PLAYER_ROT_INERTIA			(0.3f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

//*****************************************************************************
// �N���X��`
//*****************************************************************************

class Player : public Object
{
private:
	CSkinMesh		*m_CSkinMesh;		// �X�L�����b�V���i�[�p
	D3DXVECTOR3		m_vPos;				// ���W���
	D3DXVECTOR3		m_vRot;				// ��]���
	D3DXVECTOR3		m_vScl;				// �g�k���
	D3DXVECTOR3		m_vMove;			// �ړ��ʏ��
	D3DXVECTOR3		m_vAxisX;
	D3DXVECTOR3		m_vAxisZ;
	D3DXVECTOR3		m_vRotIner;			// ��]�������

	D3DXMATRIX		m_mtxWorld;			// ���[���h�}�g���N�X
	D3DXQUATERNION	m_qText;
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
	void	Move(void);
	void	MoveFunc(float);
	void	RotFunc(D3DXVECTOR3);
};

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

#endif
