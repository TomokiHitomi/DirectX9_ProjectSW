//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "calculate.h"

// �f�o�b�O�p
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

//=============================================================================
// �R���X�g���N�^�i�����������j
//=============================================================================
Player::Player(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �I�u�W�F�N�gID�ƃv���C�I���e�B�̐ݒ菈��
	SetIdAndPriority(ObjectID::PLAYER, Priority::Middle, Priority::Middle);

	// �e�v���p�e�B�̏�����
	m_vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vScl = D3DXVECTOR3(MODEL_PLAYER_SCL, MODEL_PLAYER_SCL, MODEL_PLAYER_SCL);
	m_bUse = true;

	// ���f���̏�����
	//m_CSkinMesh->Init(pDevice, "data/MODEL/Yuko.x");
	m_CSkinMesh = new CSkinMesh;
	m_CSkinMesh->Init(pDevice, MODEL_PLAYER);

}

//=============================================================================
// �f�X�g���N�^�i�I�������j
//=============================================================================
Player::~Player(void)
{
	if (m_CSkinMesh != NULL)
	{
		m_CSkinMesh->Release();
		delete m_CSkinMesh;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Player::Update(void)
{
	if (m_bUse)
	{
		// ���[���h�ϊ�
		WorldConvert(&m_mtxWorld, m_vPos, m_vRot, m_vScl);
		// �A�j���[�V�����X�V����
		m_CSkinMesh->Update(m_mtxWorld);
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void Player::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���C�e�B���O�����߂ɂ��Ă�
	//SetLight(LIGHT_SUB1, TRUE);
	//SetLight(LIGHT_SUB2, TRUE);

	if (m_bUse)
	{
		m_CSkinMesh->Draw(pDevice);
	}

	// ���C�e�B���O��ʏ�ɖ߂�
	//SetLight(LIGHT_SUB1, FALSE);
	//SetLight(LIGHT_SUB2, FALSE);
}