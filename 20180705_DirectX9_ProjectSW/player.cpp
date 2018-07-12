//=============================================================================
//
// �v���C���[���� [player.cpp]
// Author : GP12A295 25 �l���F��
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "calculate.h"
#include "input.h"
#include "camera.h"
#include "cube.h"

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
	m_vScl = D3DXVECTOR3(PLAYER_SCL, PLAYER_SCL, PLAYER_SCL);

	m_vMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vRotIner = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_bUse = true;

	// ���f���̏�����
	//m_CSkinMesh->Init(pDevice, "data/MODEL/Yuko.x");
	m_CSkinMesh = new CSkinMesh;
	m_CSkinMesh->Init(pDevice, PLAYER_MODEL);

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
#ifdef _DEBUG
	PrintDebugProc("�y MODEL �z\n");
#endif

	if (m_bUse)
	{
		// �ړ�����
		Move();

		// ���[���h�ϊ�
		WorldConvert(&m_mtxWorld, m_vPos, m_vRot, m_vScl);
		//WorldConvertPR(&m_mtxWorld, m_vPos, m_vRot, m_vScl);

		// �A�j���[�V�����X�V����
		m_CSkinMesh->Update(m_mtxWorld);
#ifdef _DEBUG
		PrintDebugProc("Pos [%f,%f,%f]\n", m_vPos.x, m_vPos.y, m_vPos.z);
		PrintDebugProc("Rot [%f,%f,%f]\n", m_vRot.x, m_vRot.y, m_vRot.z);
		PrintDebugProc("Move[%f,%f,%f]\n", m_vMove.x, m_vMove.y, m_vMove.z);
#endif
	}

#ifdef _DEBUG
	PrintDebugProc("\n");
#endif
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
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		m_CSkinMesh->Draw(pDevice);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	// ���C�e�B���O��ʏ�ɖ߂�
	//SetLight(LIGHT_SUB1, FALSE);
	//SetLight(LIGHT_SUB2, FALSE);
}

//=============================================================================
// �J�����ړ��֐�
//=============================================================================
void Player::Move(void)
{
	//// �ړ�����
	//if (InputPress(INPUT_LEFT))
	//{
	//	if (InputPress(INPUT_UP))
	//	{// ���O�ړ�
	//		MoveFunc(m_vRot.y + D3DX_PI * 0.75f);
	//	}
	//	else if (InputPress(INPUT_DOWN))
	//	{// ����ړ�
	//		MoveFunc(m_vRot.y + D3DX_PI * 0.25f);
	//	}
	//	else if (InputPress(INPUT_RIGHT))
	//	{// ���E���������͏����Ȃ�
	//	}
	//	else
	//	{// ���ړ�
	//		MoveFunc(m_vRot.y + D3DX_PI * 0.50f);
	//	}
	//}
	//else if (InputPress(INPUT_RIGHT))
	//{
	//	if (InputPress(INPUT_UP))
	//	{// �E�O�ړ�
	//		MoveFunc(m_vRot.y - D3DX_PI * 0.75f);
	//	}
	//	else if (InputPress(INPUT_DOWN))
	//	{// �E��ړ�
	//		MoveFunc(m_vRot.y - D3DX_PI * 0.25f);
	//	}
	//	else
	//	{// �E�ړ�
	//		MoveFunc(m_vRot.y - D3DX_PI * 0.50f);
	//	}
	//}
	//else if (InputPress(INPUT_UP))
	//{// �O�ړ�
	//	if (InputPress(INPUT_DOWN))
	//	{// �O�㓯�������͏����Ȃ�
	//	}
	//	else
	//	{
	//		MoveFunc(m_vRot.y + D3DX_PI);
	//	}
	//}
	//else if (GetKeyboardPress(DIK_S) || IsButtonPressed(0, BUTTON_DOWN))
	//{// ��ړ�
	//	MoveFunc(m_vRot.y);
	//}


	if (InputPress(INPUT_UP) && InputPress(INPUT_DOWN))
	{
		// ���������͏����Ȃ�
	}
	else if (InputPress(INPUT_UP))
	{
		m_vRotIner.x += PLAYER_ROT_SPEED;
	}
	else if (InputPress(INPUT_DOWN))
	{
		m_vRotIner.x -= PLAYER_ROT_SPEED;
	}

	if (InputPress(INPUT_LEFT) && InputPress(INPUT_RIGHT))
	{
		// ���������͏����Ȃ�
	}
	else if (InputPress(INPUT_LEFT))
	{
		m_vRotIner.z -= PLAYER_ROT_SPEED;
	}
	else if (InputPress(INPUT_RIGHT))
	{
		m_vRotIner.z += PLAYER_ROT_SPEED;
	}


	//// ��]�ʂ����b�g�ɓK�p
	//m_vRot += m_vRotIner;

	//// ��]������K�p
	//m_vRotIner -= m_vRotIner * PLAYER_ROT_INERTIA;

	// �J���������f���ɃZ�b�g
	D3DXVECTOR3 vTemp = m_vPos;
	vTemp.y += PLAYER_HEIGHT;
	SetCameraAt(vTemp);

	//Cube::SetPos(m_vRot);

	//// ��]���a��ݒ�
	//D3DXVECTOR3 vTa = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//D3DXVECTOR3 vZero = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 vAxis;

	//// ���݂̊p�x�ɐ����ȉ�]���x�N�g����ݒ�
	//vAxis = D3DXVECTOR3(cosf(m_vRot.z), 0, sinf(m_vRot.z));

	//// �N�H�[�^�j�I������
	//QuaternionCalculate(&vTa, &vAxis, D3DX_PI * 1.5f + m_vRot.x,
	//	&vZero);

	//// ��]���ɐݒu
	//Cube::SetPos((m_vPos + vZero) * 100.0f);
	//m_vPos += vZero;

	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	/******************** ���[���h�ϊ� ********************/
	// ���[���h�}�g���N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �yT�z���s�ړ��𔽉f(�I�u�W�F�N�g��z�u���Ă���j
	D3DXMatrixTranslation(&mtxTranslate, 0.0f, 0.0f, -1.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTranslate);

		// �yR�z��]�𔽉f(YawPitchRoll��y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, m_vRotIner.x, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_vRotIner.y, 0.0f, m_vRotIner.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);




	//D3DXVECTOR3 vPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//D3DXVECTOR3 vScl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	//WorldConvertPR(&m_mtxWorld, vPos, m_vRot, vScl);

	m_vPos += D3DXVECTOR3(
		m_mtxWorld._41,
		m_mtxWorld._42,
		m_mtxWorld._43);

	m_vRot.x = atan2(m_mtxWorld._42, m_mtxWorld._43);

	//m_vRot.y = 2 * (D3DX_PI - atan2(m_mtxWorld._41, m_mtxWorld._43));

	//m_vRot.z = 2 * (D3DX_PI - atan2(m_mtxWorld._41, m_mtxWorld._42));
}

//=============================================================================
// �ړ������֐�
//=============================================================================
void Player::MoveFunc(float fAngle)
{
	m_vPos.x += sinf(fAngle) * PLAYER_MOVE_SPEED;
	m_vPos.z += cosf(fAngle) * PLAYER_MOVE_SPEED;
}

//=============================================================================
// ��]�����֐�
//=============================================================================
void Player::RotFunc(D3DXVECTOR3 vRot)
{
	//m_vPos.x += sinf(fAngle) * PLAYER_MOVE_SPEED;
	//m_vPos.z += cosf(fAngle) * PLAYER_MOVE_SPEED;
}